#include <iostream>
#include <process.h>
#include <Windows.h>
#include <chrono>
#include <random>
using namespace std;

#pragma comment(lib, "winmm.lib")

constexpr uint32_t ce_clientCount = 5;
int g_Data = 0;
int g_Connect = 0;
bool g_ShutDown = false;

// 크리티컬 섹션
CRITICAL_SECTION cs_connect;
CRITICAL_SECTION cs_data;

// SRWLock
SRWLOCK srw_connect;
SRWLOCK srw_data;

uint32_t WINAPI acceptProc(LPVOID param)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(100, 1000);

    // 크리티컬 섹션
/*    while (!g_ShutDown)
    {
        Sleep(distrib(gen));
        EnterCriticalSection(&cs_connect);
        g_Connect++;
        LeaveCriticalSection(&cs_connect);
    }*/

    // SRWLock
    while (!g_ShutDown)
    {
        Sleep(distrib(gen));
        AcquireSRWLockExclusive(&srw_connect);
        g_Connect++;
        ReleaseSRWLockExclusive(&srw_connect);
    }

    return 0;
}

uint32_t WINAPI disconnectProc(LPVOID param)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(100, 1000);

    // 크리티컬 섹션
/*    while (!g_ShutDown)
    {
        Sleep(distrib(gen));
        EnterCriticalSection(&cs_connect);
        g_Connect--;
        LeaveCriticalSection(&cs_connect);
    }*/

    // SRWLock
    while (!g_ShutDown)
    {
        Sleep(distrib(gen));
        AcquireSRWLockExclusive(&srw_connect);
        g_Connect--;
        ReleaseSRWLockExclusive(&srw_connect);
    }

    return 0;
}

uint32_t WINAPI updateProc(LPVOID param)
{
    // 크리티컬 섹션
/*    while (!g_ShutDown)
    {
        Sleep(10);
        EnterCriticalSection(&cs_data);
        g_Data++;
        if (g_Data % 1000 == 0)
        {
            printf("data : %d\n", g_Data);
        }
        LeaveCriticalSection(&cs_data);
    }*/

    // SRWLock
    while (!g_ShutDown)
    {
        Sleep(10);
        AcquireSRWLockExclusive(&srw_data);
        g_Data++;
        if (g_Data % 1000 == 0)
        {
            printf("data : %d\n", g_Data);
        }
        ReleaseSRWLockExclusive(&srw_data);
    }

    return 0;
}

int wmain()
{
    timeBeginPeriod(1);

    // 크리티컬 섹션
    InitializeCriticalSection(&cs_connect);
    InitializeCriticalSection(&cs_data);

    // SRWLock
    InitializeSRWLock(&srw_connect);
    InitializeSRWLock(&srw_data);

    HANDLE hThread[ce_clientCount];

    hThread[0] = (HANDLE)_beginthreadex(NULL, 0, acceptProc, NULL, NULL, NULL);
    hThread[1] = (HANDLE)_beginthreadex(NULL, 0, disconnectProc, NULL, NULL, NULL);

    for (auto i = 2; i < ce_clientCount; ++i)
    {
        hThread[i] = (HANDLE)_beginthreadex(NULL, 0, updateProc, NULL, NULL, NULL);
    }

    auto start = chrono::steady_clock::now();
    while (!g_ShutDown)
    {
        Sleep(1000);
        chrono::duration<double> dur = chrono::steady_clock::now() - start;

        // 크리티컬 섹션
/*        EnterCriticalSection(&cs_connect);
        printf("connect : %d\n", g_Connect);
        LeaveCriticalSection(&cs_connect);*/

        // SRWLock
        AcquireSRWLockShared(&srw_connect);
        printf("connect : %d\n", g_Connect);
        ReleaseSRWLockShared(&srw_connect);

        if (dur.count() >= 20.0)
        {
            g_ShutDown = true;
        }
    }

    WaitForMultipleObjects(ce_clientCount, hThread, TRUE, INFINITE);

    for (auto i = 0; i < ce_clientCount; ++i)
    {
        CloseHandle(hThread[i]);
    }

    DeleteCriticalSection(&cs_connect);
    DeleteCriticalSection(&cs_data);

    return 0;
}