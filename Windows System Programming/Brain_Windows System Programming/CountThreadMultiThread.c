/*#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <tchar.h>

#define MAX_THREADS (1024*10)

unsigned int WINAPI ThreadProc(LPVOID lpParam)
{
    LPDWORD threadNum = (LPDWORD)lpParam;

    while (1)
    {
        _tprintf(_T("thread num: %d \n"), *threadNum);
        Sleep(5000);
    }

    return 0;
}

DWORD cnt = 0;
LPDWORD cntOfThread = &cnt;

int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, _T("korean"));

    DWORD dwThreadID[MAX_THREADS];
    HANDLE hThread[MAX_THREADS];

    while (1)
    {
        hThread[*cntOfThread] = (HANDLE)_beginthreadex(NULL, 0, ThreadProc,
            (LPVOID)cntOfThread, 0, (unsigned*)&dwThreadID[*cntOfThread]);

        if (hThread[*cntOfThread] == NULL)
        {
            _tprintf(_T("MAXIMUM THREAD NUMBER : %d \n"), *cntOfThread);
            break;
        }
        (*cntOfThread)++;
    }

    for (DWORD i = 0; i < *cntOfThread; i++)
    {
        CloseHandle(hThread[i]);
    }

    return 0;
}*/