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
    Parser() : chpBuff(nullptr), _chBuffStart(nullptr) {}

    ~Parser()
    {
        if (_chBuffStart)
        {
            delete[] _chBuffStart;
            _chBuffStart = nullptr;
        }
    }

    // LoadFile 에서 new[] 로 할당한 버퍼의 시작 주소
    WCHAR* _chBuffStart;
};

// 스페이스, 탭, 엔터코드, 콤마 등 의미 없는 구분자를 건너뛴다
BOOL SkipNoneCommand(void);

// 다음 단어 얻기. *chppBuffer 에는 단어의 시작 주소,
// *ipLength 에는 단어의 길이가 담겨서 돌아온다
BOOL GetNextWord(WCHAR** chppBuffer, int* ipLength);

// 따옴표로 감싸인 문자열 얻기 ("~~~")
// 따옴표 자체는 길이에 포함하지 않는다
BOOL GetStringWord(WCHAR** chppBuffer, int* ipLength);