#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <tchar.h>
#include <time.h>

#define STRING_LEN 100

void ShowAttributes(DWORD attrib);
void ShowFileTime(FILETIME t);

int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, L"korean");

    TCHAR fileName[] = _T("data.txt");
    TCHAR fileFullPathName[STRING_LEN];
    LPTSTR filePtr;

    GetFullPathName(fileName, STRING_LEN, fileFullPathName, &filePtr);

    _tprintf(_T("%s \n"), fileFullPathName);
    _tprintf(_T("%s \n"), filePtr);

    return 0;
}