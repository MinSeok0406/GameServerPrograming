#define _WIN32_WINNT0x0400
#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <tchar.h>
#include <time.h>

VOID CALLBACK APCProc(ULONG_PTR);

int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, L"korean");

    HANDLE hThread = GetCurrentThread();
    
    QueueUserAPC(APCProc, hThread, (ULONG_PTR)1);
    QueueUserAPC(APCProc, hThread, (ULONG_PTR)2);
    QueueUserAPC(APCProc, hThread, (ULONG_PTR)3);
    QueueUserAPC(APCProc, hThread, (ULONG_PTR)4);
    QueueUserAPC(APCProc, hThread, (ULONG_PTR)5);

    Sleep(5000);
    SleepEx(INFINITE, TRUE);
    return 0;
}

VOID CALLBACK APCProc(ULONG_PTR dwParam)
{
    _tprintf(_T("Asynchronous procedure call num %llu \n"), dwParam);
}