#include "LanServer.h"
#include <process.h>
#include <conio.h>

bool LanServer::Start(uint16_t port, unsigned long workerThread, bool isNagle, unsigned long NumberOfConcurrentThreads)
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 1;
    }

    hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, NumberOfConcurrentThreads - 2);
    if (hcp == NULL)
    {
        return 1;
    }

    HANDLE hThread;
    hThread = (HANDLE)_beginthreadex(NULL, 0, acceptThreadProc, hcp, 0, NULL);
    if (hThread == NULL)
    {
        return 1;
    }
    CloseHandle(hThread);

    for (auto i = 0; i < workerThread; ++i)
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

    if (!isNagle)
    {
        int flag = 1;
        setsockopt(listen_sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&flag, sizeof(flag));
    }

    SOCKADDR_IN serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(port);

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

    return true;
}

void LanServer::Stop()
{
    WaitForMultipleObjects(CLIENT, hThreads, TRUE, INFINITE);

    for (auto it = map_session.begin(); it != map_session.end();)
    {
        Session* session = it->second;
        closesocket(session->socket);
        DeleteCriticalSection(&(session->cs));
        delete session->recvOL;
        delete session->sendOL;
        delete session;
        it = map_session.erase(it);
    }

    CloseHandle(hcp);
    WSACleanup();
}

void LanServer::Update()
{
    while (true)
    {
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
}

bool LanServer::disconnect(uint64_t sessionID)
{
    AcquireSRWLockExclusive(&srw_session);
    Session* session = map_session[sessionID];
    map_session.erase(sessionID);
    ReleaseSRWLockExclusive(&srw_session);

    closesocket(session->socket);
    delete session->recvOL;
    delete session->sendOL;

    EnterCriticalSection(&(session->cs));
    LeaveCriticalSection(&(session->cs));
    DeleteCriticalSection(&(session->cs));

    delete session;

    return true;
}

bool LanServer::sendPacket(uint64_t sessionID, SerializationBuffer* packet)
{
    AcquireSRWLockExclusive(&srw_session);
    Session* session = map_session[sessionID];
    map_session.erase(sessionID);
    EnterCriticalSection(&(session->cs));
    ReleaseSRWLockExclusive(&srw_session);

    // 수정 필요
    packet->clear();
    auto ret = session->recvQ.Dequeue(packet->getBufferPtr(), session->recvQ.GetUseSize());
    packet->moveWritePos(ret);
    session->sendQ.Enqueue(packet->getBufferPtr(), packet->getDataSize());

    sendPost(session);

    LeaveCriticalSection(&(session->cs));

    return true;
}

uint32_t __stdcall LanServer::IOCPworkerThread(LPVOID arg)
{
    return 0;
}

uint32_t __stdcall LanServer::acceptThreadProc(LPVOID param)
{
    return 0;
}

void LanServer::acceptThread()
{}

bool LanServer::sendPost(Session* session)
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
                disconnect((uint64_t)session + 1);
            }
            return false;
        }
    }
    //printf("count : %lld\n", session->count);
    //printf("sendbyte : %d \t transferred : %d\n", sendRet, transferred);

    return true;
}

bool LanServer::recvPost(Session* session)
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
                disconnect((uint64_t)session + 1);
            }
            return false;
        }
    }
    //printf("count : %lld\n", session->count);
    //printf("recvbyte : %d \t transferred : %d\n", recvRet, transferred);

    return true;
}
