#include <iostream>
#include <process.h>
#include <Windows.h>
#include <chrono>
using namespace std;
using ll = long long;

#pragma comment(lib, "winmm.lib")

uint64_t COUNT = 10000000;

HANDLE hEvent;
long isLock = false;
uint64_t g_count = 0;

CRITICAL_SECTION cs;

void spinLock()
{
    while (true)
    {
        if (isLock == 0)
        {
            if (InterlockedExchange(&isLock, 1) == 0)
            {
                break;
            }
            _mm_pause();
        }
    }
}

void spinUnlock()
{
    InterlockedExchange(&isLock, 0);
}

uint32_t __stdcall threadProc(LPVOID Param)
{
    WaitForSingleObject(hEvent, INFINITE);

    for (auto i = 0; i < COUNT; ++i)
    {
        spinLock();
        //EnterCriticalSection(&cs);

        g_count++;

        //LeaveCriticalSection(&cs);
        spinUnlock();
    }

    return 0;
}

int wmain()
{
    timeBeginPeriod(1);

    InitializeCriticalSection(&cs);

    HANDLE hThread[10];

    hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    for (auto i = 0; i < 10; ++i)
    {
        hThread[i] = (HANDLE)_beginthreadex(NULL, 0, threadProc, NULL, 0, NULL);
    }

    Sleep(1000);

    SetEvent(hEvent);

    auto start = chrono::steady_clock::now();
    WaitForMultipleObjects(10, hThread, TRUE, INFINITE);
    auto end = chrono::steady_clock::now();
    chrono::duration<double, std::milli> duration_ms = end - start;

    for (auto i = 0; i < 10; ++i)
    {
        CloseHandle(hThread[i]);
    }

    CloseHandle(hEvent);

    cout << "Running Time : " << duration_ms.count() << "ms" << "\n";
    cout << g_count << "\n";

    DeleteCriticalSection(&cs);

    return 0;
}