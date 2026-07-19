/*#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <time.h>
#include <fcntl.h>
#include <io.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <string>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

#define SERVERPORT  47000
#define BUFSIZE     512

int wmain(int argc, WCHAR* argv[])
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

    timeBeginPeriod(1);
    srand(unsigned int(time(nullptr)));

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET)
    {
        return 1;
    }

    SOCKADDR_IN serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    int retval = bind(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR)
    {
        wprintf(L"%d\n", WSAGetLastError());
        return 1;
    }

    SOCKADDR_IN clientaddr;
    int addrlen;
    wchar_t buf[BUFSIZE + 1];

    while (true)
    {
        addrlen = sizeof(clientaddr);
        retval = recvfrom(sock, (char*)buf, BUFSIZE * sizeof(wchar_t), 0, (SOCKADDR*)&clientaddr, &addrlen);
        if (retval == SOCKET_ERROR)
        {
            wprintf(L"%d\n", WSAGetLastError());
            return 1;
        }

        buf[retval / sizeof(wchar_t)] = L'\0';
        wchar_t addr[INET_ADDRSTRLEN];
        InetNtop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
        wprintf(L"[UDP/%s:%d] %s\n", addr, ntohs(clientaddr.sin_port), buf);
    }

    closesocket(sock);
    WSACleanup();

    return 0;
}*/