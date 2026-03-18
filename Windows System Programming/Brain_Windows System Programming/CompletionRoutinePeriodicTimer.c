/*#define _WIN32_WINNT0x0400
#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <tchar.h>
#include <time.h>

VOID CALLBACK TimerAPCProc(LPVOID, DWORD, DWORD);

int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, L"korean");

    HANDLE hTimer = NULL;
    LARGE_INTEGER liDueTime;

    liDueTime.QuadPart = -100000000;

    hTimer = CreateWaitableTimer(NULL, FALSE, _T("WaitableTimer"));
    if (!hTimer)
    {
        _tprintf(_T("CreateWaitableTimer failed (%d) \n"), GetLastError());
        return 1;
    }

    _tprintf(_T("Waiting for 10 seconds...\n"));

    SetWaitableTimer(hTimer, &liDueTime, 5000, TimerAPCProc, _T("Timer was signaled. \n"), FALSE);

    while (1)
    {
        SleepEx(INFINITE, TRUE);
    }

    return 0;
}

VOID CALLBACK TimerAPCProc(LPVOID lpArg, DWORD timerLowVal, DWORD timerHighVal)
{
    _tprintf(_T("%s"), (TCHAR*)lpArg);
    MessageBeep(MB_ICONEXCLAMATION);
}*/