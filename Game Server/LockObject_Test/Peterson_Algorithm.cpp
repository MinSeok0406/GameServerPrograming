#include <iostream>
#include <process.h>
#include <Windows.h>
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
    bool fl;
    int t;

    flag[0] = true;
    turn = 0;

    while (true)
    {
        fl = flag[0];
        t = turn;

        if (flag[1] == false)
        {
            break;
        }

        if (turn == 1)
        {
            break;
        }
    }

    flagTemp[0] = fl;
    turnTemp = t;
}

void Lock_2()
{
    bool fl;
    int t;

    flag[1] = true;
    turn = 1;

    while (true)
    {
        fl = flag[1];
        t = turn;

        if (flag[0] == false)
        {
            break;
        }

        if (turn == 0)
        {
            break;
        }
    }

    flagTemp[1] = fl;
    turnTemp = t;
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
    // 여기에 동시에 들어오는지 확인!
    if (InterlockedAdd(&g_threadCount, 1) != 1)
    {
        cout << flagTemp[0] << " " << flagTemp[1] << "\n";
        cout << turnTemp << "\n";
        __debugbreak();
    }

    g_cnt++;

    // 먼저 들어온 스레드 잡아두기
    if (InterlockedCompareExchange(&g_threadCount, 0, 1) != 1)
    {
        while (true)
        {
            
        }
    }
}

uint32_t WINAPI threadProc1(LPVOID param)
{
    WaitForSingleObject(g_hEvent, INFINITE);

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

    WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

    CloseHandle(hThread[0]);
    CloseHandle(hThread[1]);
    
    CloseHandle(g_hEvent);

    cout << g_cnt << "\n";

    return 0;
}