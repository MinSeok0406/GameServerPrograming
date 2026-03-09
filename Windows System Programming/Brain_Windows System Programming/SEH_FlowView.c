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

    _tprintf(_T("start point! \n"));

    int* p = NULL;

    __try
    {
        *p = 100;
        _tprintf(_T("value: %d\n"), *p);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        _tprintf(_T("exception occurred! \n"));
    }

    _tprintf(_T("end point! \n"));

    return 0;
}*/