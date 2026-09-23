#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <time.h>
#include <process.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <string>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

#define SERVERPORT 47000
#define BUFSIZE 512

SOCKET g_listensock;

bool netLogic();
uint32_t WINAPI procThread(LPVOID param);

int wmain()
{
    timeBeginPeriod(1);

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("WSADATA error : %d\n", WSAGetLastError());
        return 1;
    }

    g_listensock = socket(AF_INET, SOCK_STREAM, 0);
    if (g_listensock == INVALID_SOCKET)
    {
        printf("SOCKET error : %d\n", WSAGetLastError());
        return 1;
    }

    LINGER linger{ 1, 0 };
    setsockopt(g_listensock, SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(linger));

    SOCKADDR_IN serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    auto bindRet = bind(g_listensock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (bindRet == SOCKET_ERROR)
    {
        printf("bind error : %d\n", WSAGetLastError());
        return 1;
    }

    /*u_long on = 1;
    auto ioctRet = ioctlsocket(g_listensock, FIONBIO, &on);
    if (ioctRet == SOCKET_ERROR)
    {
        printf("ioct error : %d\n", WSAGetLastError());
        return 1;
    }*/

    auto listenRet = listen(g_listensock, SOMAXCONN);
    if (listenRet == SOCKET_ERROR)
    {
        printf("listen error : %d\n", WSAGetLastError());
        return 1;
    }

    while (true)
    {
        netLogic();
    }

    closesocket(g_listensock);
    WSACleanup();

    return 0;
}

bool netLogic()
{
    SOCKET clientSock;
    SOCKADDR_IN clientaddr;
    int addrlen = sizeof(clientaddr);

    auto acceptRet = accept(clientSock, (SOCKADDR*)&clientaddr, &addrlen);
    if (acceptRet == INVALID_SOCKET)
    {
        printf("accept error : %d\n", WSAGetLastError());
        return false;
    }

    HANDLE hThread;
    hThread = (HANDLE)_beginthreadex(0, 0, procThread, (LPVOID)clientSock, 0, 0);
    if (hThread == NULL)
    {
        closesocket(clientSock);
    }
    else
    {
        CloseHandle(hThread);
    }

    return true;
}

uint32_t __stdcall procThread(LPVOID param)
{
    SOCKET clientSock = (SOCKET)param;
    char buf[BUFSIZE + 1];

    while (true)
    {
        int recvRet = recv(clientSock, buf, BUFSIZE, 0);
        if (recvRet == SOCKET_ERROR)
        {
            printf("recv error : %d\n", WSAGetLastError());
            break;
        }
        else if (recvRet == 0)
        {
            break;
        }

        buf[recvRet] = '\0';

        int sendRet = send(clientSock, buf, recvRet, 0);
        if (sendRet == SOCKET_ERROR)
        {
            printf("send error : %d\n", WSAGetLastError());
            break;
        }
    }

    closesocket(clientSock);

    return 0;
}
