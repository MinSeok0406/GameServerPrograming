/*#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <tchar.h>

UINT WINAPI ThreadProc(LPVOID lpParam)
{
    while (1)
    {
        _tprintf(_T("Running State! \n"));
        Sleep(10000);
    }

    return 0;
}

int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, _T("korean"));

    DWORD dwThreadID;
    HANDLE hThread;
    DWORD susCnt;

    hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadProc,
        NULL, CREATE_SUSPENDED, (unsigned*)&dwThreadID);

    if (hThread == NULL)
    {
        _tprintf(_T("Thread creation fault! \n"));
    }

    susCnt = ResumeThread(hThread);
    _tprintf(_T("suspend count: %d \n"), susCnt);
    Sleep(10000);

    susCnt = SuspendThread(hThread);
    _tprintf(_T("suspend count: %d \n"), susCnt);
    Sleep(10000);

    susCnt = SuspendThread(hThread);
    _tprintf(_T("suspend count: %d \n"), susCnt);
    Sleep(10000);

    susCnt = ResumeThread(hThread);
    _tprintf(_T("suspend count: %d \n"), susCnt);

    susCnt = ResumeThread(hThread);
    _tprintf(_T("suspend count: %d \n"), susCnt);

    WaitForSingleObject(hThread, INFINITE);

    return 0;
}*/