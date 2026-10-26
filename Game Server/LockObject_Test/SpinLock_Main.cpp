/*#include <iostream>
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

void CriticalSection()
{
    g_count++;
}

uint32_t __stdcall threadProc1(LPVOID Param)
{
    WaitForSingleObject(hEvent, INFINITE);

    SetThreadAffinityMask(GetCurrentThread(), 1ULL << 10);

    for (auto i = 0; i < COUNT; ++i)
    {
        spinLock();
        CriticalSection();
        spinUnlock();
    }

    return 0;
}

uint32_t __stdcall threadProc2(LPVOID Param)
{
    WaitForSingleObject(hEvent, INFINITE);

    SetThreadAffinityMask(GetCurrentThread(), 1ULL << 10);

    for (auto i = 0; i < COUNT; ++i)
    {
        spinLock();
        CriticalSection();
        spinUnlock();
    }

    return 0;
}

int wmain()
{
    timeBeginPeriod(1);

    HANDLE hThread[2];

    hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    hThread[0] = (HANDLE)_beginthreadex(NULL, 0, threadProc1, NULL, 0, NULL);
    hThread[1] = (HANDLE)_beginthreadex(NULL, 0, threadProc2, NULL, 0, NULL);

    Sleep(1000);

    SetEvent(hEvent);

    auto start = chrono::steady_clock::now();
    WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
    chrono::duration<double, std::milli> duration_ms = chrono::steady_clock::now() - start;

    for (auto i = 0; i < 2; ++i)
    {
        CloseHandle(hThread[i]);
    }

    CloseHandle(hEvent);

    cout << "Running Time : " << duration_ms.count() << "ms" << "\n";
    cout << g_count << "\n";

    return 0;
}*/