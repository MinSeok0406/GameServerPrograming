/*#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <tchar.h>
#include <time.h>

#define BEST_TEST_EXCEPTION ((DWORD) 0xE0000008L)

void SoftwareException(void);

int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, L"korean");

    SoftwareException();
    _tprintf(_T("End of the _tmain \n"));

    return 0;
}

void SoftwareException(void)
{
    __try
    {
        RaiseException(BEST_TEST_EXCEPTION, 0, NULL, NULL);
        _tprintf(_T("It's work! \n"));
    }
    __except (EXCEPTION_CONTINUE_EXECUTION)
    {
        DWORD exptType = GetExceptionCode();
        if (exptType == BEST_TEST_EXCEPTION)
        {
            _tprintf(_T("BEST_TEST_EXCEPTION exception occurred\n"));
        }
    }
}*/