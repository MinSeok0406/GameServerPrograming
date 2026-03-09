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

    int a, b;

    __try
    {
        _tprintf(_T("input divide string [ a / b ]: "));
        _tscanf(_T("%d / %d"), &a, &b);

        if (b == 0)
            return -1;
    }
    __finally
    {
        _tprintf(_T("__finally block! \n"));
    }

    _tprintf(_T("result : %d \n"), a / b);

    return 0;
}*/