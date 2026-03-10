/*#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <tchar.h>
#include <time.h>

#define STATUS_DEFAULT_USER_DEFINED_EXPT ((DWORD)0xE0000008L)

void SoftwareException(void);
DWORD FilterFunction(LPEXCEPTION_POINTERS exptPtr);

int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, L"korean");

    SoftwareException();
    _tprintf(_T("End of the _tmain \n"));

    return 0;
}

void SoftwareException(void)
{
    TCHAR* exptString[] = {
        _T("Exception String One!"),
        _T("Exception String Two!"),
        _T("Exception String Three!")
    };

    __try
    {
        RaiseException(STATUS_DEFAULT_USER_DEFINED_EXPT, 0, 3, (ULONG_PTR*)exptString);
    }
    __except (FilterFunction(GetExceptionInformation()))
    {
        DWORD exptType = GetExceptionCode();
        _tprintf(_T("Recv: exception code: 0x%x \n"), exptType);
    }
}

DWORD FilterFunction(LPEXCEPTION_POINTERS exptPtr)
{
    PEXCEPTION_RECORD exptRecord = exptPtr->ExceptionRecord;

    switch (exptRecord->ExceptionCode)
    {
    case STATUS_DEFAULT_USER_DEFINED_EXPT:
        _tprintf(_T("STATUS_DEFAULT_USER_DEFINED_EXPT exception \n"));
        _tprintf(_T("Exception code: 0x%x \n"), exptRecord->ExceptionCode);
        _tprintf(_T("Exception flags: %d \n"), exptRecord->ExceptionFlags);
        _tprintf(_T("Exception parm num: %d \n"), exptRecord->NumberParameters);
        _tprintf(_T("String one: %s \n"), (TCHAR*)exptRecord->ExceptionInformation[0]);
        _tprintf(_T("String Two: %s \n"), (TCHAR*)exptRecord->ExceptionInformation[1]);
        _tprintf(_T("String Three: %s \n"), (TCHAR*)exptRecord->ExceptionInformation[2]);
        break;
    default:
        break;
    }

    return EXCEPTION_EXECUTE_HANDLER;
}*/