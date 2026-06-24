#pragma once
#include <Windows.h>

class Parser
{
public:
    BOOL LoadFile(const char* str);
    BOOL GetValue(const char* str, int* value);

private:
    Parser() = default;
    ~Parser() = default;
};

// 스페이스, 탭, 엔터코드, 주석 처리
BOOL SkipNoneCommand(void);

// 다음 단어 얻기
BOOL GetNextWord(char** chppBuffer, int* ipLength);

// 문자열 얻기
BOOL GetStringWord(char** chppBuffer, int* ipLength);