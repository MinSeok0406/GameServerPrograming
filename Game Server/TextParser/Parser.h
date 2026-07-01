#pragma once
#include <Windows.h>

class Parser
{
public:
    BOOL LoadFile(const WCHAR* str);
    BOOL GetValue(const WCHAR* str, int* value);

    static Parser* GetInstance()
    {
        if (_pParser == nullptr)
        {
            _pParser = new Parser();
        }

        return _pParser;
    }

    void DestoryInstance()
    {
        if (_pParser)
        {
            delete _pParser;
            _pParser = nullptr;
        }
    }

    WCHAR* chpBuff;

private:
    static Parser* _pParser;
    Parser() = default;
    ~Parser() = default;
};

// 스페이스, 탭, 엔터코드, 주석 처리
BOOL SkipNoneCommand(void);

// 다음 단어 얻기
BOOL GetNextWord(WCHAR** chppBuffer, int* ipLength);

// 문자열 얻기
BOOL GetStringWord(WCHAR** chppBuffer, int* ipLength);