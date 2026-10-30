#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <time.h>
#include <chrono>
#include <vector>
#include <algorithm>
#include <process.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

#define SERVERPORT 47000
#define BUFSIZE 512

struct SOCKETINFO
{
    WSAOVERLAPPED overlapped;
    SOCKET sock;
    char buf[BUFSIZE + 1];
    int recvbytes;
    int sendbytes;
    WSABUF wsabuf;
};

SOCKET client_sock;
HANDLE hReadEvent;
HANDLE hWriteEvent;

uint32_t WINAPI workerthread(LPVOID arg);
void CALLBACK completionRoutine(DWORD dwError, DWORD cbTransferred,
    LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);

int wmain()
{
    timeBeginPeriod(1);
    srand((uint32_t)time(nullptr));

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 1;
    }

    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET)
    {
        printf("socket : %d\n", WSAGetLastError());
        return 1;
    }

    //int newSize = 0;
    //setsockopt(listen_sock, SOL_SOCKET, SO_SNDBUF, (const char*)&newSize, sizeof(newSize));

    SOCKADDR_IN serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);

    int bindRet = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (bindRet == SOCKET_ERROR)
    {
        printf("bind : %d\n", WSAGetLastError());
        return 1;
    }

    int listenRet = listen(listen_sock, SOMAXCONN);
    if (listenRet == SOCKET_ERROR)
    {
        printf("listen : %d\n", WSAGetLastError());
        return 1;
    }

    //u_long on = 1;
    //auto ioctRet = ioctlsocket(listen_sock, FIONBIO, &on);

    hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
    hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, workerthread, NULL, 0, NULL);
    CloseHandle(hThread);

    while (true)
    {
        WaitForSingleObject(hReadEvent, INFINITE);
        client_sock = accept(listen_sock, NULL, NULL);
        if (client_sock == INVALID_SOCKET)
        {
            printf("accept : %d\n", WSAGetLastError());
            break;
        }

        SetEvent(hWriteEvent);
    }
    
    closesocket(listen_sock);
    WSACleanup();

    return 0;
}

uint32_t __stdcall workerthread(LPVOID arg)
{
    while (true)
    {
        while (true)
        {
            unsigned long waitRet = WaitForSingleObjectEx(hWriteEvent, INFINITE, TRUE);

            if (waitRet == WAIT_OBJECT_0)
            {
                break;
            }

            if (waitRet != WAIT_IO_COMPLETION)
            {
                return 1;
            }
        }

        SOCKADDR_IN clientaddr;
        int addrlen = sizeof(clientaddr);
        getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

        SOCKETINFO* ptr = new SOCKETINFO;
        memset(&(ptr->overlapped), 0, sizeof(ptr->overlapped));
        ptr->sock = client_sock;
        SetEvent(hReadEvent);
        ptr->recvbytes = 0;
        ptr->sendbytes = 0;
        ptr->wsabuf.buf = ptr->buf;
        ptr->wsabuf.len = BUFSIZE;

        unsigned long recvbytes;
        unsigned long flags = 0;
        auto recvRet = WSARecv(ptr->sock, &(ptr->wsabuf), 1, &recvbytes, &flags, &(ptr->overlapped), completionRoutine);
        if (recvRet == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSA_IO_PENDING)
            {
                return 1;
            }
        }
    }

    return 0;
}

void completionRoutine(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
{
    SOCKETINFO* ptr = (SOCKETINFO*)lpOverlapped;
    SOCKADDR_IN clientaddr;
    int addrlen = sizeof(clientaddr);
    getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addrlen);

    if (dwError != 0 || cbTransferred == 0)
    {
        closesocket(ptr->sock);
        delete ptr;
        return;
    }

    if (ptr->recvbytes == 0)
    {
        ptr->recvbytes = cbTransferred;
        ptr->sendbytes = 0;

        ptr->buf[ptr->recvbytes] = '\0';
    }
    else
    {
        ptr->sendbytes += cbTransferred;
    }

    if (ptr->recvbytes > ptr->sendbytes)
    {
        memset(&(ptr->overlapped), 0, sizeof(ptr->overlapped));
        ptr->wsabuf.buf = ptr->buf + ptr->sendbytes;
        ptr->wsabuf.len = ptr->recvbytes - ptr->sendbytes;

        unsigned long sendbytes;
        auto sendRet = WSASend(ptr->sock, &(ptr->wsabuf), 1, &sendbytes, 0, &(ptr->overlapped), completionRoutine);
        if (sendRet == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSA_IO_PENDING)
            {
                return;
            }
        }
    }
    else
    {
        ptr->recvbytes = 0;

        memset(&(ptr->overlapped), 0, sizeof(ptr->overlapped));
        ptr->wsabuf.buf = ptr->buf;
        ptr->wsabuf.len = BUFSIZE;

        unsigned long recvbytes;
        unsigned long flags = 0;
        auto recvRet = WSARecv(ptr->sock, &(ptr->wsabuf), 1, &recvbytes, &flags, &(ptr->overlapped), completionRoutine);
        if (recvRet == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSA_IO_PENDING)
            {
                return;
            }
        }
    }
}