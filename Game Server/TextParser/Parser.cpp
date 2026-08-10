#include "Parser.h"
#include <cstdio>
#include <tchar.h>

Parser* Parser::_pParser = nullptr;

extern Parser* parser;

BOOL Parser::LoadFile(const WCHAR* str)
{
    FILE* fp;
    _wfopen_s(&fp, str, L"rb");
    if (fp == nullptr)
    {
        return FALSE;
    }

    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (fileSize <= 0)
    {
        fclose(fp);
        return FALSE;
    }

    // 파일이 유니코드(UTF - 16 LE) 텍스트로 저장되어 있다는 전제로
    // WCHAR(2바이트) 단위로 그대로 읽어들인다
    int wcharCount = fileSize / sizeof(WCHAR);

    if (_chBuffStart)
    {
        delete[] _chBuffStart;
        _chBuffStart = nullptr;
    }

    _chBuffStart = new WCHAR[wcharCount + 1];

    auto readCount = fread(_chBuffStart, sizeof(WCHAR), wcharCount, fp);
    fclose(fp);

    WCHAR* start = _chBuffStart;
    if (readCount > 0 && start[0] == 0xFEFF)
    {
        start++; // 유니코드 BOM 건너뛰기
    }

    chpBuff = start; // 파싱 커서를 버퍼 시작으로 초기화

    return TRUE;
}

BOOL Parser::GetValue(const WCHAR* str, int* value)
{
    WCHAR chWord[256];
    WCHAR* pWord = nullptr;
    int iLength = 0;

    while (GetNextWord(&pWord, &iLength))
    {
        wmemcpy(chWord, pWord, iLength);
        chWord[iLength] = L'\0';

        if (0 == wcscmp(str, chWord))
        {
            if (GetNextWord(&pWord, &iLength))
            {
                wmemcpy(chWord, pWord, iLength);
                chWord[iLength] = L'\0';

                if (0 == wcscmp(chWord, L"="))
                {
                    if (GetNextWord(&pWord, &iLength))
                    {
                        wmemcpy(chWord, pWord, iLength);
                        chWord[iLength] = L'\0';
                        *value = _wtoi(chWord);
                        return TRUE;
                    }
                }
            }
            return FALSE;
        }
    }

    return FALSE;
}

BOOL SkipNoneCommand(void)
{
    WCHAR* buffer = parser->chpBuff;

    while (*buffer == L',' || *buffer == 0x20 || *buffer == 0x08 ||
        *buffer == 0x09 || *buffer == 0x0a || *buffer == 0x0d)
    {
        buffer++;   // 구분자인 동안 계속 넘어간다
    }

    parser->chpBuff = buffer;

    return TRUE;
}

BOOL GetNextWord(WCHAR** chppBuffer, int* ipLength)
{
    SkipNoneCommand();

    WCHAR* buffer = parser->chpBuff;

    if (*buffer == L'\0')
    {
        return FALSE; // 더 이상 읽을 내용이 없음 (파일 끝)
    }

    if (*buffer == L'"')
    {
        // 따옴표로 시작하면 문자열 전용 처리로 넘긴다.
        return GetStringWord(chppBuffer, ipLength);
    }

    WCHAR* start = buffer;

    while (*buffer != L',' && *buffer != L'"' && *buffer != 0x20 &&
        *buffer != 0x08 && *buffer != 0x09 && *buffer != 0x0a &&
        *buffer != 0x0d && *buffer != L'\0')
    {
        buffer++;
    }

    *chppBuffer = start;
    *ipLength = (int)(buffer - start);

    parser->chpBuff = buffer;

    return TRUE;
}

BOOL GetStringWord(WCHAR** chppBuffer, int* ipLength)
{
    WCHAR* buffer = parser->chpBuff;

    if (*buffer != L'"')
    {
        return FALSE;
    }

    buffer++; // 여는 따옴표 건너뛰기
    WCHAR* start = buffer;

    while (*buffer != L'"' && *buffer != L'\0')
    {
        buffer++;
    }

    if (*buffer != L'"')
    {
        return FALSE; // 닫는 따옴표를 못 찾음 — 파일 형식이 잘못됨
    }

    *chppBuffer = start;
    *ipLength = (int)(buffer - start);

    parser->chpBuff = buffer + 1; // 닫는 따옴표 다음부터 다시 시작

    return TRUE;
}
