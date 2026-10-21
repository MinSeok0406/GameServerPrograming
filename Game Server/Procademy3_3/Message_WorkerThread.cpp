#include <iostream>
#include <process.h>
#include <Windows.h>
#include <string>
#include <list>
#include <random>
#include <conio.h>
#include <chrono>
#include "RingBuffer.h"
using namespace std;

#pragma comment(lib, "winmm.lib")

#define CLIENT      10
#define dfJOB_ADD	0
#define dfJOB_DEL	1
#define dfJOB_SORT	2
#define dfJOB_FIND	3
#define dfJOB_PRINT	4	
#define dfJOB_QUIT	5

struct HEADER
{
    uint16_t _type;
    uint16_t _payloadLen;
};

wstring g_msg[4] = { L"HELLO", L"MY NAME IS", L"MINSEOK!", L"NICE TO MEET YOU" };

list<wstring> g_List;
RingBuffer g_msgQ(40000);

HANDLE g_hEvent[2];

SRWLOCK g_msgQSRW;
SRWLOCK g_listSRW;

uint64_t g_TPS;
uint64_t g_addTPS;
uint64_t g_delTPS;
uint64_t g_sortTPS;
uint64_t g_findTPS;
uint64_t g_printTPS;

uint32_t WINAPI workerProc(LPVOID param)
{
    printf("Start ThreadID : %d\n", GetCurrentThreadId());

    while (true)
    {
        auto waitRet = WaitForMultipleObjects(2, g_hEvent, FALSE, INFINITE);

        if (waitRet - WAIT_OBJECT_0 == 0)
        {
            AcquireSRWLockExclusive(&g_msgQSRW);
            g_TPS++;
            HEADER header;
            auto dequeueRet = g_msgQ.Dequeue((char*)&header, sizeof(HEADER));
            if (dequeueRet == 0)
            {
                ReleaseSRWLockExclusive(&g_msgQSRW);
                continue;
            }

            if (header._payloadLen == 0)
            {
                ReleaseSRWLockExclusive(&g_msgQSRW);
            }

            if (header._type == dfJOB_ADD)
            {
                uint16_t payload = header._payloadLen;
                wchar_t buf[128];
                auto dqRet = g_msgQ.Dequeue((char*)buf, payload);
                if (dqRet == 0)
                {
                    __debugbreak();
                }
                g_addTPS++;
                ReleaseSRWLockExclusive(&g_msgQSRW);

                wstring msg(buf, payload / sizeof(wchar_t));
                AcquireSRWLockExclusive(&g_listSRW);
                g_List.push_back(msg);
                ReleaseSRWLockExclusive(&g_listSRW);
            }
            else if (header._type == dfJOB_DEL)
            {
                AcquireSRWLockExclusive(&g_listSRW);
                if (!g_List.empty())
                {
                    g_List.pop_back();
                }
                g_delTPS++;
                ReleaseSRWLockExclusive(&g_listSRW);
            }
            else if (header._type == dfJOB_SORT)
            {
                AcquireSRWLockExclusive(&g_listSRW);
                g_List.sort();
                g_sortTPS++;
                ReleaseSRWLockExclusive(&g_listSRW);
            }
            else if (header._type == dfJOB_FIND)
            {
                AcquireSRWLockExclusive(&g_listSRW);
                auto it = std::find(g_List.begin(), g_List.end(), L"MY NAME IS");
                /*if (it != g_List.end())
                {
                    printf("FIND!\n");
                }
                else
                {
                    printf("NOT FIND!\n");
                }*/
                g_findTPS++;
                ReleaseSRWLockExclusive(&g_listSRW);
            }
            else if (header._type == dfJOB_PRINT)
            {
                AcquireSRWLockExclusive(&g_listSRW);
                list<wstring> temp = g_List;
                g_printTPS++;
                ReleaseSRWLockExclusive(&g_listSRW);
                /*for (auto l : temp)
                {
                    printf("%ls -", l.c_str());
                }
                printf("\n");*/
            }
        }
        else if (waitRet - WAIT_OBJECT_0 == 1)
        {
            // 종료 코드 작성
            // 종료 메시지 코드를 어떻게 처리할지...
            break;
        }

    }

    printf("End ThreadID : %d\n", GetCurrentThreadId());

    return 0;
}

uint32_t WINAPI logProc(LPVOID param)
{
    printf("Start LogThreadID : %d\n", GetCurrentThreadId());

    auto t = chrono::steady_clock::now();
    while (true)
    {
        auto waitRet = WaitForSingleObject(g_hEvent[1], 1000);
        if (waitRet == WAIT_OBJECT_0)
        {
            break;
        }
        
        printf("msgQ_size : %d\n", g_msgQ.GetUseSize());
        AcquireSRWLockShared(&g_msgQSRW);
        printf("TPS : %llu\n", g_TPS);
        printf("ADD_TYPE : %llu\n", g_addTPS);
        printf("DEL_TYPE : %llu\n", g_delTPS);
        printf("SORT_TYPE : %llu\n", g_sortTPS);
        printf("FIND_TYPE : %llu\n", g_findTPS);
        printf("PRINT_TYPE : %llu\n", g_printTPS);
        g_TPS = 0;
        g_addTPS = 0;
        g_delTPS = 0;
        g_sortTPS = 0;
        g_findTPS = 0;
        g_printTPS = 0;
        ReleaseSRWLockShared(&g_msgQSRW);
    }

    printf("End LogThreadID : %d\n", GetCurrentThreadId());

    return 0;
}

int wmain()
{
    timeBeginPeriod(1);
    InitializeSRWLock(&g_msgQSRW);
    InitializeSRWLock(&g_listSRW);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution rng(0, 4);
    uniform_int_distribution rn(0, 3);

    HANDLE hThread[CLIENT];
    HANDLE hLogThread;
    g_hEvent[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
    g_hEvent[1] = CreateEvent(NULL, TRUE, FALSE, NULL);

    for (auto i = 0; i < CLIENT; ++i)
    {
        hThread[i] = (HANDLE)_beginthreadex(NULL, 0, workerProc, NULL, NULL, NULL);
    }

    hLogThread = (HANDLE)_beginthreadex(NULL, 0, logProc, NULL, NULL, NULL);

    while (true)
    {
        auto type = rng(gen);
        if (type == dfJOB_ADD)
        {
            int index = rn(gen);
            wstring& buf = g_msg[index];
            HEADER header;
            header._type = type;
            header._payloadLen = (uint16_t)(buf.length() * sizeof(wchar_t));

            if (g_msgQ.Enqueue((char*)&header, sizeof(header)) != sizeof(header))
            {
                __debugbreak();
            }

            if (g_msgQ.Enqueue((char*)buf.c_str(), header._payloadLen) != header._payloadLen)
            {
                __debugbreak();
            }

            SetEvent(g_hEvent[0]);
        }
        else
        {
            HEADER header;
            header._type = type;
            header._payloadLen = 0;

            if (g_msgQ.Enqueue((char*)&header, sizeof(header)) == 0)
            {
                __debugbreak();
            }

            SetEvent(g_hEvent[0]);
        }

        if (_kbhit())
        {
            int key = _getch();

            if (key == 'q' || key == 'Q')
            {
                HEADER header;
                header._type = dfJOB_QUIT;
                header._payloadLen = 0;

                for (auto i = 0; i < CLIENT; ++i)
                {
                    if (g_msgQ.Enqueue((char*)&header, sizeof(header)) == 0)
                    {
                        __debugbreak();
                    }
                }

                SetEvent(g_hEvent[1]);
                break;
            }
        }

        Sleep(5);
    }

    WaitForMultipleObjects(CLIENT, hThread, TRUE, INFINITE);

    for (auto i = 0; i < CLIENT; ++i)
    {
        CloseHandle(hThread[i]);
    }

    CloseHandle(hLogThread);
    CloseHandle(g_hEvent[0]);
    CloseHandle(g_hEvent[1]);

    return 0;
}