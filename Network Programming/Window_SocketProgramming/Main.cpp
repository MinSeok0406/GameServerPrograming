/*#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <time.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <windowsx.h>
#include <list>
#include <string>
#include "RingBuffer.h"
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")



int wmain(int argc, WCHAR* argv[])
{
    timeBeginPeriod(1);
    srand(unsigned int(time(nullptr)));

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 1;
    }

    

    WSACleanup();

    return 1;
}*/