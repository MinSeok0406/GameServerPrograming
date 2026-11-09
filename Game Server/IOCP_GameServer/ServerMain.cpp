#include <iostream>
#include <process.h>
#include <chrono>
#include <conio.h>
#include <map>
#include "protocol.h"
using namespace std;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

#define SERVERPORT 6000
#define CLIENT 2

SOCKET listen_sock;
map<uint64_t, Session*> g_session;

SRWLOCK srw_session;
HANDLE event_accept;
HANDLE hThreads[CLIENT];

uint32_t WINAPI acceptWorkerThread(LPVOID arg);
uint32_t WINAPI IOCPworkerThread(LPVOID arg);

bool releaseSession(Session* session);
bool sendPost(Session* session, int transferred);
bool recvPost(Session* session, int transferred);
bool sendPacket(Session* session, int transferred);

int wmain()
{
    timeBeginPeriod(1);
    InitializeSRWLock(&srw_session);

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 1;
    }

    SYSTEM_INFO si;
    GetSystemInfo(&si);

    HANDLE hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, si.dwNumberOfProcessors - 2);
    if (hcp == NULL)
    {
        return 1;
    }

    HANDLE hThread;
    hThread = (HANDLE)_beginthreadex(NULL, 0, acceptWorkerThread, hcp, 0, NULL);
    if (hThread == NULL)
    {
        return 1;
    }
    CloseHandle(hThread);

    for (auto i = 0; i < CLIENT; ++i)
    {
        hThreads[i] = (HANDLE)_beginthreadex(NULL, 0, IOCPworkerThread, hcp, 0, NULL);
        if (hThreads[i] == NULL)
        {
            return 1;
        }
    }

    event_accept = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (event_accept == NULL)
    {
        return 1;
    }

    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET)
    {
        printf("socket : %d\n", WSAGetLastError());
        return 1;
    }

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

    SetEvent(event_accept);

    while (true)
    {
        // 종료 메시지 보낼 기능 추가
        if (_kbhit())
        {
            int ch = _getch();

            if (ch == 'q' || ch == 'Q')
            {
                closesocket(listen_sock);
                for (auto i = 0; i < CLIENT; ++i)
                {
                    PostQueuedCompletionStatus(hcp, 0, 0, NULL);
                }
                break;
            }
        }

        Sleep(20);
    }

    WaitForMultipleObjects(CLIENT, hThreads, TRUE, INFINITE);

    for (auto it = g_session.begin(); it != g_session.end();)
    {
        Session* session = it->second;
        closesocket(session->socket);
        DeleteCriticalSection(&(session->cs));
        delete session->recvOL;
        delete session->sendOL;
        delete session;
        it = g_session.erase(it);
    }

    CloseHandle(hcp);
    WSACleanup();

    return 0;
}

uint32_t __stdcall acceptWorkerThread(LPVOID arg)
{
    WaitForSingleObject(event_accept, INFINITE);

    HANDLE hcp = (HANDLE)arg;
    SOCKET client_sock;

    while (true)
    {
        SOCKADDR_IN clientaddr;
        int addrlen = sizeof(clientaddr);

        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET)
        {
            printf("accept : %d\n", WSAGetLastError());
            break;
        }

        Session* session = new Session;
        session->socket = client_sock;
        InetNtop(AF_INET, &clientaddr.sin_addr, session->ip, sizeof(session->ip));
        session->port = ntohs(clientaddr.sin_port);
        session->count = 0;
        session->sendflag = false;

        InitializeCriticalSection(&(session->cs));

        session->sendOL = new SendOverlapped;
        memset(&(session->sendOL->overlapped), 0, sizeof(session->sendOL->overlapped));
        session->sendOL->sessionID = (uint32_t)TYPE::SEND;

        session->recvOL = new RecvOverlapped;
        memset(&(session->recvOL->overlapped), 0, sizeof(session->recvOL->overlapped));
        session->recvOL->sessionID = (uint32_t)TYPE::RECV;

        WSABUF wsabuf;
        wsabuf.buf = session->recvQ.GetRearBufferPtr();
        wsabuf.len = session->recvQ.DirectEnqueueSize();

        CreateIoCompletionPort((HANDLE)session->socket, hcp, (uint64_t)session, 0);

        AcquireSRWLockExclusive(&srw_session);
        g_session[(uint64_t)session + 1] = session;
        ReleaseSRWLockExclusive(&srw_session);
        
        recvPost(session, 0);
    }

    return 0;
}

uint32_t __stdcall IOCPworkerThread(LPVOID arg)
{
    HANDLE hcp = (HANDLE)arg;
    Session* session;

    while (true)
    {
        unsigned long transferred;
        LPOVERLAPPED overlapped;
        auto GQCSret = GetQueuedCompletionStatus(hcp, &transferred, (uint64_t*)&session, &overlapped, INFINITE);
        if (GQCSret == TRUE && overlapped == NULL)
        {
            // 스레드 종료
            printf("WorkerThread Exit\n");
            break;
        }

        if (GQCSret == FALSE || transferred == 0)
        {
            // 클라가 RST 보냄 -> 소켓 정리
            // 클라 정상 종료
            printf("Client Exit\n");
        }
        else
        {
            EnterCriticalSection(&(session->cs));
            if (&(session->recvOL->overlapped) == overlapped)
            {
                session->recvQ.MoveRear(transferred);
                sendPacket(session, transferred);
                recvPost(session, transferred);
            }
            else if (&(session->sendOL->overlapped) == overlapped)
            {
                session->sendQ.MoveFront(transferred);
                session->sendflag = false;
                sendPost(session, transferred);
            }
            LeaveCriticalSection(&(session->cs));
        }

        if (InterlockedDecrement(&(session->count)) == 0)
        {
            printf("Delete session\n");
            printf("count : %lld\n", session->count);
            releaseSession(session);
        }
    }

    return 0;
}

bool releaseSession(Session* session)
{
    AcquireSRWLockExclusive(&srw_session);
    g_session.erase((uint64_t)session + 1);
    ReleaseSRWLockExclusive(&srw_session);

    closesocket(session->socket);
    delete session->recvOL;
    delete session->sendOL;
    DeleteCriticalSection(&(session->cs));

    delete session;

    return true;
}

bool sendPost(Session* session, int transferred)
{
    if (session->sendflag == true)
    {
        return false;
    }

    if (session->sendQ.GetUseSize() == 0)
    {
        return false;
    }

    session->sendflag = true;

    memset(&(session->sendOL->overlapped), 0, sizeof(session->sendOL->overlapped));
    WSABUF sendWsaBuf;
    sendWsaBuf.buf = session->sendQ.GetFrontBufferPtr();
    sendWsaBuf.len = session->sendQ.DirectDequeueSize();

    InterlockedIncrement(&(session->count));

    auto sendRet = WSASend(session->socket, &sendWsaBuf, 1, NULL, 0, &(session->sendOL->overlapped), NULL);
    if (sendRet == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSA_IO_PENDING)
        {
            session->sendflag = false;
            if (InterlockedDecrement(&(session->count)) == 0)
            {
                releaseSession(session);
            }
            return false;
        }
    }
    //printf("count : %lld\n", session->count);
    //printf("sendbyte : %d \t transferred : %d\n", sendRet, transferred);

    return true;
}

bool recvPost(Session* session, int transferred)
{
    memset(&(session->recvOL->overlapped), 0, sizeof(session->recvOL->overlapped));
    WSABUF recvWsaBuf;
    recvWsaBuf.buf = session->recvQ.GetRearBufferPtr();
    recvWsaBuf.len = session->recvQ.DirectEnqueueSize();

    InterlockedIncrement(&(session->count));

    unsigned long flags = 0;
    auto recvRet = WSARecv(session->socket, &recvWsaBuf, 1, NULL, &flags, &(session->recvOL->overlapped), NULL);
    if (recvRet == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSA_IO_PENDING)
        {
            if (InterlockedDecrement(&(session->count)) == 0)
            {
                releaseSession(session);
            }
            return false;
        }
    }
    //printf("count : %lld\n", session->count);
    //printf("recvbyte : %d \t transferred : %d\n", recvRet, transferred);

    return true;
}

bool sendPacket(Session* session, int transferred)
{
    char temp[65535];
    session->recvQ.Dequeue(temp, transferred);
    session->sendQ.Enqueue(temp, transferred);

    sendPost(session, transferred);

    return true;
}
