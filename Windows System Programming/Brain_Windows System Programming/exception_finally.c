/*#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <tchar.h>
#include <time.h>



int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, _T("korean"));

    TCHAR str[2];

    __try
    {
        _tcscpy(str, _T("ABCDEFDKWLJSDJLKIERJABKLJB"));
        _tprintf(_T("%s \n"), str);
    }
    __finally
    {
        _tprintf(_T("__finally block! \n"));
    }

    return 0;
}*/