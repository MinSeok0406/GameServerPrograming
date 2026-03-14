/*#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <tchar.h>
#include <time.h>

#define CREATE_DIRECTORY 0

int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, L"korean");

    TCHAR dirReletivePath[] = _T("GoodDirectoryOne");
    TCHAR dirFullPath[] = _T("C:\\GoodDirectoryTwo");

#if CREATE_DIRECTORY
    CreateDirectory(dirReletivePath, NULL);
    CreateDirectory(dirFullPath, NULL);
#else
    RemoveDirectory(dirReletivePath);
    RemoveDirectory(dirFullPath);
#endif

    return 0;
}*/