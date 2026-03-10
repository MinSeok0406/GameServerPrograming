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
    _tsetlocale(LC_ALL, L"korean");

    int* p = NULL;
    int sel = 1;

    while (1)
    {
        _tprintf(_T("1 for memory access exception \n"));
        _tprintf(_T("2 for divide by 0 exception \n"));
        _tprintf(_T("Select exception type[3 for exit]: "));

        _tscanf(_T("%d"), &sel);

        if (sel == 3)
            break;

        __try
        {
            if (sel == 1)
            {
                *p = 100;
                _tprintf(_T("value: %d \n"), *p);
            }
            else
            {
                int n = 0;
                n = 7 / n;
            }
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
            DWORD exptType = GetExceptionCode();

            switch (exptType)
            {
            case EXCEPTION_ACCESS_VIOLATION:
                _tprintf(_T("Access violation \n\n"));
                break;

            case EXCEPTION_INT_DIVIDE_BY_ZERO:
                _tprintf(_T("Divide by zero \n\n"));
                break;
            }
        }
    }


    return 0;
}*/