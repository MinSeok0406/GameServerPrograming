/*#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <tchar.h>


int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, _T("korean"));

    TCHAR string[] = _T("Hey, get a life! You don't even have two pennies to rub together.");
    TCHAR seps[] = _T(" ,.!");

    TCHAR* token = _tcstok(string, seps);

    while (token != NULL)
    {
        _tprintf(_T("%s\n"), token);
        token = _tcstok(NULL, seps);
    }

    return 0;
}*/