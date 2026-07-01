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
        // 에러 코드
    }

    auto result = fread(chpBuff, sizeof(str), 1, fp);
    if (result != 1)
    {
        // 에러 코드
    }

    fclose(fp);

    return 0;
}

BOOL Parser::GetValue(const WCHAR* str, int* value)
{
    WCHAR chWord[256];
    int iLength;

    while (GetNextWord(&chpBuff, &iLength))
    {
        wmemset(chWord, 0, 256);
        wmemcpy(chWord, chpBuff, iLength);

        if (0 == wcscmp(str, chWord))
        {
            if (GetNextWord(&chpBuff, &iLength))
            {
                wmemset(chWord, 0, 256);
                wmemcpy(chWord, chpBuff, iLength);
                if (0 == wcscmp(chWord, L"="))
                {
                    if (GetNextWord(&chpBuff, &iLength))
                    {
                        wmemset(chWord, 0, 256);
                        wmemcpy(chWord, chpBuff, iLength);
                        *value = _wtoi(chWord);
                        return TRUE;
                    }
                    return FALSE;
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

    while (true)
    {
        if (*buffer == ',' || *buffer == '"' || *buffer == 0x20 ||
            *buffer == 0x08 || *buffer == 0x09 || *buffer == 0x0a ||
            *buffer == 0x0d)
        {
            break;
        }

        buffer++;
    }

    return 0;
}

BOOL GetNextWord(WCHAR** chppBuffer, int* ipLength)
{
    



    return 0;
}

BOOL GetStringWord(WCHAR** chppBuffer, int* ipLength)
{
    return 0;
}
