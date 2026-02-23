/*#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <tchar.h>
#include <time.h>

#define NUM_OF_CUSTOMER 50
#define RANGE_MIN 10
#define RANGE_MAX (30 - RANGE_MIN)
#define TABLE_CNT 10

HANDLE hSemaphore;
DWORD randTimeArr[50];

void TakeMeal(DWORD time)
{
    WaitForSingleObject(hSemaphore, INFINITE);
    _tprintf(_T("Enter Customer %d~  \n"), GetCurrentThreadId());

    _tprintf(_T("Customer %d having launch~ \n"), GetCurrentThreadId());

    Sleep(1000 * time);

    ReleaseSemaphore(hSemaphore, 1, NULL);
    _tprintf(_T("Out Customer %d~  \n\n"), GetCurrentThreadId());
}

unsigned int WINAPI ThreadProc(LPVOID lpParam)
{
    TakeMeal((DWORD)lpParam);
    return 0;
}

int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, _T("korean"));

    DWORD dwThreadID[NUM_OF_CUSTOMER];
    HANDLE hThreads[NUM_OF_CUSTOMER];

    srand((unsigned)time(NULL));

    for (int i = 0; i < NUM_OF_CUSTOMER; i++)
    {
        randTimeArr[i] = (DWORD)((double)rand() / (double)RAND_MAX) * RANGE_MAX + RANGE_MIN;
    }

    hSemaphore = CreateSemaphore(NULL, TABLE_CNT, TABLE_CNT, NULL);

    if (hSemaphore == NULL)
    {
        _tprintf(_T("CreateSemaphore error: %d\n"), GetLastError());
    }

    for (int i = 0; i < NUM_OF_CUSTOMER; i++)
    {
        hThreads[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadProc,
            (void*)randTimeArr[i], CREATE_SUSPENDED, (unsigned*)&dwThreadID[i]);

        if (hThreads[i] == NULL)
        {
            _tprintf(_T("Thread creation fault! \n"));
            return -1;
        }
    }

    for (int i = 0; i < NUM_OF_CUSTOMER; i++)
    {
        ResumeThread(hThreads[i]);
    }

    WaitForMultipleObjects(NUM_OF_CUSTOMER, hThreads, TRUE, INFINITE);

    _tprintf(_T("---------END---------"));

    for (int i = 0; i < NUM_OF_CUSTOMER; i++)
    {
        CloseHandle(hThreads[i]);
    }

    CloseHandle(hSemaphore);

    return 0;
}*/