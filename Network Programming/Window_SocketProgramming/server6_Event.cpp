/*#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <time.h>
#include <fcntl.h>
#include <io.h>
#include <process.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <string>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

#define SERVERPORT  47000
#define BUFSIZE     10

HANDLE hWriteEvent;
HANDLE hReadEvent;
int buf[BUFSIZE];

unsigned int WINAPI writeThread(LPVOID arg)
{
    DWORD retval;
    for (auto k = 1; k <= 500; ++k)
    {
        retval = WaitForSingleObject(hReadEvent, INFINITE);
        if (retval != WAIT_OBJECT_0)
        {
            break;
        }

        for (auto i = 0; i < BUFSIZE; ++i)
        {
            buf[i] = k;
        }

        SetEvent(hWriteEvent);
    }

    return 0;
}

unsigned int WINAPI readThread(LPVOID arg)
{
    DWORD retval;

    while (true)
    {
        retval = WaitForSingleObject(hWriteEvent, INFINITE);
        if (retval != WAIT_OBJECT_0)
        {
            break;
        }

        wprintf(L"Thread %4d:\t", GetCurrentThreadId());
        for (auto i = 0; i < BUFSIZE; ++i)
        {
            wprintf(L"%3d ", buf[i]);
        }
        wprintf(L"\n");
        memset(buf, 0, sizeof(buf));

        SetEvent(hReadEvent);
    }

    return 0;
}

int wmain(int argc, WCHAR* argv[])
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

    timeBeginPeriod(1);
    srand(unsigned int(time(nullptr)));

    hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    hReadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    HANDLE hThread[3];
    for (auto i = 0; i < 1; ++i)
    {
        hThread[i] = (HANDLE)_beginthreadex(NULL, 0, writeThread, NULL, 0, NULL);
    }

    for (auto i = 1; i < 3; ++i)
    {
        hThread[i] = (HANDLE)_beginthreadex(NULL, 0, readThread, NULL, 0, NULL);
    }

    SetEvent(hReadEvent);

    WaitForMultipleObjects(3, hThread, TRUE, INFINITE);

    CloseHandle(hReadEvent);
    CloseHandle(hWriteEvent);

    return 0;
}*/