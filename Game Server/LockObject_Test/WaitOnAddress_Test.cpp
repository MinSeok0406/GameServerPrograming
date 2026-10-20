#include <iostream>
#include <process.h>
#include <Windows.h>
#include <chrono>
using namespace std;
using ll = long long;

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "synchronization.lib")

long g_lock = 0;

void Lock()
{
    long comp = 0;

    while (true)
    {
        if (InterlockedExchange(&g_lock, 1) == 0)
        {
            return;
        }

        WaitOnAddress(&g_lock, &comp, sizeof(int), INFINITE);
    }
}

void Unlock()
{
    WakeByAddressSingle(&g_lock);
    InterlockedExchange(&g_lock, 0);
}

uint32_t WINAPI testproc(LPVOID param)
{
    while (true)
    {
        Lock();

        printf("Thread ID : %d\n", GetCurrentThreadId());

        Unlock();
    }

    return 0;
}

int wmain()
{
    timeBeginPeriod(1);

    HANDLE hThread[3];

    for (auto i = 0; i < 3; ++i)
    {
        hThread[i] = (HANDLE)_beginthreadex(NULL, 0, testproc, NULL, NULL, NULL);
    }

    WaitForMultipleObjects(3, hThread, TRUE, INFINITE);

    for (auto i = 0; i < 3; ++i)
    {
        CloseHandle(hThread[i]);
    }

    return 0;
}