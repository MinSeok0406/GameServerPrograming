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

enum class TYPE
{
    SEND,
    RECV
};

struct SendOverlapped
{
    WSAOVERLAPPED overlapped;
    uint32_t sessionID;
    char buf[BUFSIZE + 1];
    int sendbytes;
    WSABUF wsabuf;
    TYPE type;
};

struct RecvOverlapped
{
    WSAOVERLAPPED overlapped;
    uint32_t sessionID;
    char buf[BUFSIZE + 1];
    int recvbytes;
    WSABUF wsabuf;
    TYPE type;
};

struct SOCKETINFO
{
    SendOverlapped* sendOL;
    RecvOverlapped* recvOL;
    SOCKET sock;
};

uint32_t WINAPI sendworkerthread(LPVOID arg);
uint32_t WINAPI recvworkerthread(LPVOID arg);

int wmain()
{
    timeBeginPeriod(1);
    srand((uint32_t)time(nullptr));

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 1;
    }

    HANDLE hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 3);
    if (hcp == NULL)
    {
        return 1;
    }

    SYSTEM_INFO si;
    GetSystemInfo(&si);

    HANDLE hThread;
    for (auto i = 0; i < (int)si.dwNumberOfProcessors; ++i)
    {
        hThread = (HANDLE)_beginthreadex(NULL, 0, sendworkerthread, hcp, 0, NULL);
        if (hThread == NULL)
        {
            return 1;
        }
        CloseHandle(hThread);
    }

    for (auto i = 0; i < (int)si.dwNumberOfProcessors; ++i)
    {
        hThread = (HANDLE)_beginthreadex(NULL, 0, recvworkerthread, hcp, 0, NULL);
        if (hThread == NULL)
        {
            return 1;
        }
        CloseHandle(hThread);
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

    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;
    unsigned long recvbytes;
    unsigned long flags;

    while (true)
    {
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET)
        {
            break;
        }

        CreateIoCompletionPort((HANDLE)client_sock, hcp, client_sock, 0);

        SOCKETINFO* ptr = new SOCKETINFO;
        if (ptr == NULL)
        {
            break;
        }

        ptr->sendOL = new SendOverlapped;
        ptr->recvOL = new RecvOverlapped;
        memset(&(ptr->sendOL->overlapped), 0, sizeof(ptr->sendOL->overlapped));
        memset(&(ptr->recvOL->overlapped), 0, sizeof(ptr->recvOL->overlapped));
        ptr->sock = client_sock;
        ptr->sendOL->sendbytes = 0;
        ptr->recvOL->recvbytes = 0;
        ptr->recvOL->wsabuf.buf = ptr->recvOL->buf;
        ptr->recvOL->wsabuf.len = BUFSIZE;

        flags = 0;
        auto recvRet = WSARecv(client_sock, &(ptr->recvOL->wsabuf), 1, &recvbytes, &flags, &(ptr->recvOL->overlapped), NULL);
        if (recvRet == SOCKET_ERROR)
        {
            if (WSAGetLastError() != ERROR_IO_PENDING)
            {

            }
            continue;
        }
    }
    
    closesocket(listen_sock);
    WSACleanup();

    return 0;
}

// 읽기용 스레드
uint32_t __stdcall recvworkerthread(LPVOID arg)
{
    HANDLE hcp = (HANDLE)arg;
    while (true)
    {
        unsigned long cbTransferred;
        SOCKET client_sock;
        SOCKETINFO* ptr;
        auto GQCSret = GetQueuedCompletionStatus(hcp, &cbTransferred, &client_sock, (LPOVERLAPPED*)&ptr, INFINITE);

        if (GQCSret == 0 || cbTransferred == 0)
        {
            if (GQCSret == 0)
            {
                unsigned long temp1;
                unsigned long temp2;
                WSAGetOverlappedResult(ptr->sock, &(ptr->overlapped), &temp1, FALSE, &temp2);
            }
            closesocket(ptr->sock);
            delete ptr;
            continue;
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
            auto sendRet = WSASend(ptr->sock, &(ptr->wsabuf), 1, &sendbytes, 0, &(ptr->overlapped), NULL);
            if (sendRet == SOCKET_ERROR)
            {
                if (WSAGetLastError() != WSA_IO_PENDING)
                {

                }
                continue;
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
            auto recvRet = WSARecv(ptr->sock, &(ptr->wsabuf), 1, &recvbytes, &flags, &(ptr->overlapped), NULL);
            if (recvRet == SOCKET_ERROR)
            {
                if (WSAGetLastError() != WSA_IO_PENDING)
                {

                }
                continue;
            }
        }
    }

    return 0;
}

// 쓰기용 스레드
uint32_t __stdcall sendworkerthread(LPVOID arg)
{
    HANDLE hcp = (HANDLE)arg;
    while (true)
    {
        unsigned long cbTransferred;
        SOCKET client_sock;
        SOCKETINFO* ptr;
        auto GQCSret = GetQueuedCompletionStatus(hcp, &cbTransferred, &client_sock, (LPOVERLAPPED*)&ptr, INFINITE);

        if (GQCSret == 0 || cbTransferred == 0)
        {
            if (GQCSret == 0)
            {
                unsigned long temp1;
                unsigned long temp2;
                WSAGetOverlappedResult(ptr->sock, &(ptr->overlapped), &temp1, FALSE, &temp2);
            }
            closesocket(ptr->sock);
            delete ptr;
            continue;
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
            auto sendRet = WSASend(ptr->sock, &(ptr->wsabuf), 1, &sendbytes, 0, &(ptr->overlapped), NULL);
            if (sendRet == SOCKET_ERROR)
            {
                if (WSAGetLastError() != WSA_IO_PENDING)
                {

                }
                continue;
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
            auto recvRet = WSARecv(ptr->sock, &(ptr->wsabuf), 1, &recvbytes, &flags, &(ptr->overlapped), NULL);
            if (recvRet == SOCKET_ERROR)
            {
                if (WSAGetLastError() != WSA_IO_PENDING)
                {

                }
                continue;
            }
        }
    }

    return 0;
}