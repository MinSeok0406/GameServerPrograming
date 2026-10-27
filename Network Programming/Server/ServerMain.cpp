#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <time.h>
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

int nTotalSockets = 0;
SOCKETINFO* socketInfoArray[WSA_MAXIMUM_WAIT_EVENTS];
WSAEVENT eventArray[WSA_MAXIMUM_WAIT_EVENTS];
CRITICAL_SECTION cs;

uint32_t WINAPI workerthread(LPVOID arg);
int32_t addSocketInfo(SOCKET sock);
void removeSocketInfo(int nIndex);

int wmain()
{
    timeBeginPeriod(1);
    srand((uint32_t)time(nullptr));

    InitializeCriticalSection(&cs);

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

    int newSize = 0;
    setsockopt(listen_sock, SOL_SOCKET, SO_SNDBUF, (const char*)&newSize, sizeof(newSize));

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

    WSAEVENT hEvent = WSACreateEvent();
    eventArray[nTotalSockets++] = hEvent;

    HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, workerthread, NULL, 0, NULL);
    CloseHandle(hThread);

    SOCKET clientsock;
    SOCKADDR_IN clientaddr;
    int addrlen;
    unsigned long recvbytes, flags;

    while (true)
    {
        addrlen = sizeof(clientaddr);
        clientsock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (clientsock == INVALID_SOCKET)
        {
            break;
        }

        if (addSocketInfo(clientsock) == FALSE)
        {
            closesocket(clientsock);
            continue;
        }

        SOCKETINFO* ptr = socketInfoArray[nTotalSockets - 1];
        flags = 0;
        int recvRet = WSARecv(ptr->sock, &(ptr->wsabuf), 1, &recvbytes, &flags, &(ptr->overlapped), NULL);
        if (recvRet == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSA_IO_PENDING)
            {
                removeSocketInfo(nTotalSockets - 1);
                continue;
            }
            else if (WSAGetLastError() == WSA_IO_PENDING)
            {
                printf("recv IO PENDING\n");
            }
        }

        WSASetEvent(eventArray[0]);
    }
    
    closesocket(listen_sock);
    DeleteCriticalSection(&cs);
    WSACleanup();

    return 0;
}

uint32_t __stdcall workerthread(LPVOID arg)
{
    while (true)
    {
        auto index = WSAWaitForMultipleEvents(nTotalSockets, eventArray, FALSE, WSA_INFINITE, FALSE);
        if (index == WSA_WAIT_FAILED)
        {
            continue;
        }

        index -= WSA_WAIT_EVENT_0;
        WSAResetEvent(eventArray[index]);
        if (index == 0)
        {
            continue;
        }

        SOCKETINFO* ptr = socketInfoArray[index];
        SOCKADDR_IN clientaddr;
        int addrlen = sizeof(clientaddr);
        getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addrlen);

        unsigned long cbTransferred, flags;
        int olRet = WSAGetOverlappedResult(ptr->sock, &(ptr->overlapped), &cbTransferred, FALSE, &flags);
        if (olRet == FALSE || cbTransferred == 0)
        {
            removeSocketInfo(index);
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
            ptr->overlapped.hEvent = eventArray[index];
            ptr->wsabuf.buf = ptr->buf + ptr->sendbytes;
            ptr->wsabuf.len = ptr->recvbytes - ptr->sendbytes;

            unsigned long sendbytes;
            int sendRet = WSASend(ptr->sock, &(ptr->wsabuf), 1, &sendbytes, 0, &(ptr->overlapped), NULL);
            if (sendRet == SOCKET_ERROR)
            {
                if (WSAGetLastError() == WSA_IO_PENDING)
                {
                    printf("send IO PENDING\n");
                }

                continue;
            }
        }
        else
        {
            ptr->recvbytes = 0;

            memset(&(ptr->overlapped), 0, sizeof(ptr->overlapped));
            ptr->overlapped.hEvent = eventArray[index];
            ptr->wsabuf.buf = ptr->buf;
            ptr->wsabuf.len = BUFSIZE;

            unsigned long recvbytes;
            flags = 0;
            int recvRet = WSARecv(ptr->sock, &(ptr->wsabuf), 1, &recvbytes, &flags, &(ptr->overlapped), NULL);
            if (recvRet == SOCKET_ERROR)
            {
                if (WSAGetLastError() == WSA_IO_PENDING)
                {
                    printf("recv IO PENDING\n");
                }

                continue;
            }
        }
    }

    return 0;
}

int32_t addSocketInfo(SOCKET sock)
{
    EnterCriticalSection(&cs);
    if (nTotalSockets >= WSA_MAXIMUM_WAIT_EVENTS)
    {
        LeaveCriticalSection(&cs);
        return FALSE;
    }

    SOCKETINFO* ptr = new SOCKETINFO;
    if (ptr == NULL)
    {
        LeaveCriticalSection(&cs);
        return FALSE;
    }

    WSAEVENT hEvent = WSACreateEvent();
    if (hEvent == WSA_INVALID_EVENT)
    {
        LeaveCriticalSection(&cs);
        return FALSE;
    }

    memset(&(ptr->overlapped), 0, sizeof(ptr->overlapped));
    ptr->overlapped.hEvent = hEvent;
    ptr->sock = sock;
    ptr->recvbytes = ptr->sendbytes = 0;
    ptr->wsabuf.buf = ptr->buf;
    ptr->wsabuf.len = BUFSIZE;
    socketInfoArray[nTotalSockets] = ptr;
    eventArray[nTotalSockets] = hEvent;
    ++nTotalSockets;

    LeaveCriticalSection(&cs);
    return TRUE;
}

void removeSocketInfo(int nIndex)
{
    EnterCriticalSection(&cs);

    SOCKETINFO* ptr = socketInfoArray[nIndex];
    closesocket(ptr->sock);
    delete ptr;
    WSACloseEvent(eventArray[nIndex]);

    if (nIndex != (nTotalSockets - 1))
    {
        socketInfoArray[nIndex] = socketInfoArray[nTotalSockets - 1];
        eventArray[nIndex] = eventArray[nTotalSockets - 1];
    }
    --nTotalSockets;

    LeaveCriticalSection(&cs);
}