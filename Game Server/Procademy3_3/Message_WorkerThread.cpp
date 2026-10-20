#include <iostream>
#include <process.h>
#include <Windows.h>
#include <string>
#include <list>
#include <random>
#include <conio.h>
#include "RingBuffer.h"
using namespace std;

#pragma comment(lib, "winmm.lib")

#define CLIENT      3
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

uint32_t WINAPI workerProc(LPVOID param)
{
    printf("ThreadID : %d\n", GetCurrentThreadId());

    while (true)
    {
        auto waitRet = WaitForMultipleObjects(2, g_hEvent, FALSE, INFINITE);

        if (waitRet - WAIT_OBJECT_0 == 0)
        {
            HEADER header;
            g_msgQ.Dequeue((char*)&header, sizeof(HEADER));

            if (header._type == dfJOB_ADD)
            {
                uint16_t payload = header._payloadLen;

            }
            else if (header._type == dfJOB_DEL)
            {

            }
            else if (header._type == dfJOB_SORT)
            {

            }
            else if (header._type == dfJOB_FIND)
            {

            }
            else if (header._type == dfJOB_PRINT)
            {

            }
        }
        else if (waitRet - WAIT_OBJECT_0 == 1)
        {
            // 종료 코드 작성
            break;
        }

    }

    return 0;
}

uint32_t WINAPI logProc(LPVOID param)
{
    printf("ThreadID : %d\n", GetCurrentThreadId());


    return 0;
}

int wmain()
{
    timeBeginPeriod(1);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution rng(0, 4);
    uniform_int_distribution rn(0, 3);

    HANDLE hThread[CLIENT];
    g_hEvent[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
    g_hEvent[1] = CreateEvent(NULL, TRUE, FALSE, NULL);

    for (auto i = 0; i < CLIENT; ++i)
    {
        hThread[i] = (HANDLE)_beginthreadex(NULL, 0, workerProc, NULL, NULL, NULL);
    }

    while (true)
    {
        auto type = rng(gen);
        if (type == dfJOB_ADD)
        {
            int index = rn(gen);
            wstring& buf = g_msg[index];
            HEADER header;
            header._type = type;
            header._payloadLen = buf.length() * sizeof(wchar_t);

            if (g_msgQ.Enqueue((char*)&header, sizeof(header)) != sizeof(header))
            {
                __debugbreak();
            }

            if (g_msgQ.Enqueue((char*)buf.c_str(), buf.length() * sizeof(wchar_t)) != header._payloadLen)
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

        Sleep(50);
    }

    WaitForMultipleObjects(CLIENT, hThread, TRUE, INFINITE);

    for (auto i = 0; i < CLIENT; ++i)
    {
        CloseHandle(hThread[i]);
    }

    CloseHandle(g_hEvent[0]);
    CloseHandle(g_hEvent[1]);

    return 0;
}