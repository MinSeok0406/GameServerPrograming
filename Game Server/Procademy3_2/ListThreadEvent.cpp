#include <iostream>
#include <process.h>
#include <Windows.h>
#include <list>
#include <conio.h>
#include <random>
using namespace std;

#pragma comment(lib, "winmm.lib")

constexpr uint32_t ce_clientCount = 5;
list<int> g_datalist;

HANDLE g_hEvent;
HANDLE g_hSaveEvent;

SRWLOCK srw_data;

uint32_t WINAPI printProc(LPVOID param)
{
    while (true)
    {
        auto waitRet = WaitForSingleObject(g_hEvent, 1000);

        if (waitRet == WAIT_OBJECT_0)
        {
            break;
        }
        else if (waitRet == WAIT_TIMEOUT)
        {
            AcquireSRWLockShared(&srw_data);
            for (auto& data : g_datalist)
            {
                printf("%d-", data);
            }
            printf("\n");
            ReleaseSRWLockShared(&srw_data);
        }
    }

    return 0;
}

uint32_t WINAPI deleteProc(LPVOID param)
{
    while (true)
    {
        auto waitRet = WaitForSingleObject(g_hEvent, 333);

        if (waitRet == WAIT_OBJECT_0)
        {
            break;
        }
        else if (waitRet == WAIT_TIMEOUT)
        {
            AcquireSRWLockExclusive(&srw_data);
            if (!g_datalist.empty())
            {
                g_datalist.pop_back();
            }
            
            ReleaseSRWLockExclusive(&srw_data);
        }
    }

    return 0;
}

uint32_t WINAPI workerProc(LPVOID param)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(1, 1000);

    while (true)
    {
        auto waitRet = WaitForSingleObject(g_hEvent, 1000);

        if (waitRet == WAIT_OBJECT_0)
        {
            break;
        }
        else if (waitRet == WAIT_TIMEOUT)
        {
            AcquireSRWLockExclusive(&srw_data);
            g_datalist.push_back(distrib(gen));
            ReleaseSRWLockExclusive(&srw_data);
        }
    }

    return 0;
}

uint32_t WINAPI saveProc(LPVOID param)
{
    WaitForSingleObject(g_hSaveEvent, INFINITE);

    AcquireSRWLockExclusive(&srw_data);
    
    Sleep(2000);

    ReleaseSRWLockExclusive(&srw_data);

    return 0;
}

int wmain()
{
    timeBeginPeriod(1);
    InitializeSRWLock(&srw_data);

    HANDLE hThread[ce_clientCount];
    HANDLE hSaveThread;

    g_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    g_hSaveEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    hThread[0] = (HANDLE)_beginthreadex(NULL, 0, printProc, NULL, NULL, NULL);
    hThread[1] = (HANDLE)_beginthreadex(NULL, 0, deleteProc, NULL, NULL, NULL);

    for (auto i = 2; i < ce_clientCount; ++i)
    {
        hThread[i] = (HANDLE)_beginthreadex(NULL, 0, workerProc, NULL, NULL, NULL);
    }

    hSaveThread = (HANDLE)_beginthreadex(NULL, 0, saveProc, NULL, NULL, NULL);

    while (true)
    {
        if (_kbhit())
        {
            auto key = _getch();
            if (key == 's')
            {
                SetEvent(g_hSaveEvent);
            }
            else if (key == 'e')
            {
                SetEvent(g_hEvent);
                break;
            }
        }
    }

    WaitForMultipleObjects(ce_clientCount, hThread, TRUE, INFINITE);
    for (auto i = 0; i < ce_clientCount; ++i)
    {
        CloseHandle(hThread[i]);
    }

    CloseHandle(hSaveThread);

    CloseHandle(g_hEvent);
    CloseHandle(g_hSaveEvent);

    return 0;
}