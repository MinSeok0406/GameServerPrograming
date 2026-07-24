#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
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
#define BUFSIZE     512

struct SOCKETINFO
{
    WSAOVERLAPPED overlapped;
    SOCKET sock;
    char buf[BUFSIZE + 1];
    int recvBytes;
    int sendBytes;
    WSABUF wsabuf;
};

int nTotalSockets = 0;
SOCKETINFO* socketInfoArray[WSA_MAXIMUM_WAIT_EVENTS];
WSAEVENT eventArray[WSA_MAXIMUM_WAIT_EVENTS];
CRITICAL_SECTION cs;

u_int WINAPI workerThread(LPVOID arg);
BOOL AddSocketInfo(SOCKET sock);
void RemoveSocketInfo(int nIndex);

int wmain(int argc, WCHAR* argv[])
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    timeBeginPeriod(1);
    srand(unsigned int(time(nullptr)));

    InitializeCriticalSection(&cs);

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 1;
    }

    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET)
    {
        wprintf(L"%d\n", WSAGetLastError());
        return 0;
    }

    SOCKADDR_IN serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    int bindRet = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (bindRet == SOCKET_ERROR)
    {
        wprintf(L"%d\n", WSAGetLastError());
        return 0;
    }

    int listenRet = listen(listen_sock, SOMAXCONN);
    if (listenRet == SOCKET_ERROR)
    {
        wprintf(L"%d\n", WSAGetLastError());
        return 0;
    }

    WSAEVENT hEvent = WSACreateEvent();
    if (hEvent == WSA_INVALID_EVENT)
    {
        wprintf(L"%d\n", WSAGetLastError());
        return 0;
    }
    eventArray[nTotalSockets++] = hEvent;

    HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, workerThread, NULL, 0, NULL);
    if (hThread == NULL)
    {
        return 0;
    }
    CloseHandle(hThread);

    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;
    u_int recvbytes, flags;

    while (true)
    {
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET)
        {
            wprintf(L"%d\n", WSAGetLastError());
            break;
        }
    }
    
    closesocket(listen_sock);
    WSACleanup();

    return 1;
}

u_int WINAPI workerThread(LPVOID arg)
{
    return 0;
}

BOOL AddSocketInfo(SOCKET sock)
{
    return 0;
}

void RemoveSocketInfo(int nIndex)
{
}
