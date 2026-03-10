/*#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <tchar.h>
#include <time.h>

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
    DWORD DefineException = 0x00;

    DefineException |= 0x01 << 31;
    DefineException |= 0x01 << 30;

    DefineException |= 0x01 << 29;

    DefineException |= 0x00 << 28;

    DefineException |= 0x00 << 16;

    DefineException |= 0x08;

    __try
    {
        _tprintf(_T("Send: exception code: 0x%x \n"), DefineException);
        RaiseException(DefineException, 0, NULL, NULL);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        DWORD exptType = GetExceptionCode();
        _tprintf(_T("Recv: exception code: 0x%x \n"), exptType);
    }
}*/