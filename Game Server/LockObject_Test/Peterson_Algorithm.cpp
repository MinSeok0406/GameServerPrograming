#include <iostream>
#include <process.h>
#include <Windows.h>
#include <chrono>
using namespace std;
using ll = long long;

#pragma comment(lib, "winmm.lib")

#define COUNT 10000000

uint32_t g_cnt = 0;
bool flag[2] = { false, false };
int turn = -1;

bool flagTemp[2] = { false, false };
int turnTemp = -1;

long g_threadCount = 0;

HANDLE g_hEvent;

void Lock_1()
{
    flag[0] = true;
    turn = 0;
    MemoryBarrier();

    while (true)
    {
        if (flag[1] == false)
        {
            break;
        }

        if (turn == 1)
        {
            break;
        }
    }
}

void Lock_2()
{
    flag[1] = true;
    turn = 1;
    MemoryBarrier();

    while (true)
    {

        if (flag[0] == false)
        {
            break;
        }

        if (turn == 0)
        {
            break;
        }
    }
}

void Unlock_1()
{
    flag[0] = false;
}

void Unlock_2()
{
    flag[1] = false;
}

void CriticalSection()
{
    g_cnt++;
}

uint32_t WINAPI threadProc1(LPVOID param)
{
    WaitForSingleObject(g_hEvent, INFINITE);

    SetThreadAffinityMask(GetCurrentThread(), 1 << 1);

    for (auto i = 0; i < COUNT; ++i)
    {
        Lock_1();
        CriticalSection();
        Unlock_1();
    }

    return 0;
}

uint32_t WINAPI threadProc2(LPVOID param)
{
    WaitForSingleObject(g_hEvent, INFINITE);

    SetThreadAffinityMask(GetCurrentThread(), 1 << 2);

    for (auto i = 0; i < COUNT; ++i)
    {
        Lock_2();
        CriticalSection();
        Unlock_2();
    }

    return 0;
}

int wmain()
{
    timeBeginPeriod(1);

    HANDLE hThread[2];

    g_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    hThread[0] = (HANDLE)_beginthreadex(NULL, 0, threadProc1, NULL, NULL, NULL);
    hThread[1] = (HANDLE)_beginthreadex(NULL, 0, threadProc2, NULL, NULL, NULL);

    Sleep(100);

    SetEvent(g_hEvent);

    auto start = chrono::steady_clock::now();
    WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
    chrono::duration<double, milli> dur = chrono::steady_clock::now() - start;

    CloseHandle(hThread[0]);
    CloseHandle(hThread[1]);
    
    CloseHandle(g_hEvent);

    cout << g_cnt << "\n";
    cout << dur.count() << "ms" << "\n";

    return 0;
}