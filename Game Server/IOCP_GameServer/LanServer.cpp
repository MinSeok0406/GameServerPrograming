#include "LanServer.h"
#include <process.h>
#include <conio.h>

bool LanServer::Start(uint16_t port, unsigned long workerThread, bool isNagle, unsigned long NumberOfConcurrentThreads)
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return false;
    }

    hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, NumberOfConcurrentThreads - 2);
    if (hcp == NULL)
    {
        return false;
    }

    for (uint32_t i = 0; i < workerThread; ++i)
    {
        hThreads[i] = (HANDLE)_beginthreadex(NULL, 0, IOCPThreadProc, this, 0, NULL);
        if (hThreads[i] == NULL)
        {
            return false;
        }
    }

    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET)
    {
        printf("socket : %d\n", WSAGetLastError());
        return false;
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
        return false;
    }

    int listenRet = listen(listen_sock, SOMAXCONN);
    if (listenRet == SOCKET_ERROR)
    {
        printf("listen : %d\n", WSAGetLastError());
        return false;
    }

    HANDLE hThread;
    hThread = (HANDLE)_beginthreadex(NULL, 0, acceptThreadProc, this, 0, NULL);
    if (hThread == NULL)
    {
        return false;
    }
    CloseHandle(hThread);

    return true;
}

void LanServer::Stop()
{
    WaitForMultipleObjects(CLIENT, hThreads, TRUE, INFINITE);

    for (auto it = map_session.begin(); it != map_session.end();)
    {
        Session* session = it->second;
        closesocket(session->socket);
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
    auto it = map_session.find(sessionID);
    if (it == map_session.end())
    {
        ReleaseSRWLockExclusive(&srw_session);
        return false;
    }
    Session* session = it->second;
    ReleaseSRWLockExclusive(&srw_session);

    CancelIoEx((HANDLE)session->socket, NULL);

    return true;
}

bool LanServer::sendPacket(uint64_t sessionID, SerializationBuffer* packet)
{
    AcquireSRWLockExclusive(&srw_session);
    auto it = map_session.find(sessionID);
    if (it == map_session.end())
    {
        ReleaseSRWLockExclusive(&srw_session);
        return false;
    }
    Session* session = it->second;
    ReleaseSRWLockExclusive(&srw_session);

    // 수정 필요
    int size = packet->getDataSize();
    int enqRet = session->sendQ.Enqueue(packet->getBufferPtr(), size);
    //printf("[send] dataSize:%d enqueueRet:%d sendQ use:%d\n", size, enqRet, session->sendQ.GetUseSize());
    sendPost(session);

    return true;
}

uint32_t __stdcall LanServer::IOCPThreadProc(LPVOID arg)
{
    auto thread = static_cast<LanServer*>(arg);
    thread->IOCPThread();

    return 0;
}

uint32_t __stdcall LanServer::acceptThreadProc(LPVOID param)
{
    auto thread = static_cast<LanServer*>(param);
    thread->acceptThread();

    return 0;
}

void LanServer::IOCPThread()
{
    Session* session;

    while (true)
    {
        unsigned long transferred;
        LPOVERLAPPED overlapped;
        auto GQCSret = GetQueuedCompletionStatus(hcp, &transferred, (uint64_t*)&session, &overlapped, INFINITE);
        //printf("GQCS ret:%d transferred:%lu ol:%p\n", GQCSret, transferred, overlapped);

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
            disconnect(session->sessionID);
            printf("Client Exit\n");
        }
        else
        {
            if (&(session->recvOL->overlapped) == overlapped)
            {
                session->recvQ.MoveRear(transferred);
                //printf("[recv] transferred:%lu recvQ use:%d\n", transferred, session->recvQ.GetUseSize());

                SerializationBuffer packet;
                packet.clear();
                auto dequeueRet = session->recvQ.Dequeue(packet.getBufferPtr(), session->recvQ.GetUseSize());
                packet.moveReadPos(dequeueRet);
                //printf("[recv] dequeueRet:%d packet size:%d\n", dequeueRet, packet.getDataSize());

                OnRecv(session->sessionID, &packet);

                recvPost(session);
            }
            else if (&(session->sendOL->overlapped) == overlapped)
            {
                session->sendQ.MoveFront(transferred);
                InterlockedExchange(&(session->sendflag), 0);
                sendPost(session);
            }
        }

        if (InterlockedDecrement(&(session->count)) == 0)
        {
            printf("Delete session\n");
            printf("count : %d\n", session->count);
            releaseSession(session);
        }
    }
}

void LanServer::acceptThread()
{
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

        wchar_t ip[INET_ADDRSTRLEN];
        InetNtop(AF_INET, &clientaddr.sin_addr, ip, INET_ADDRSTRLEN);
        uint16_t port = ntohs(clientaddr.sin_port);

        if (!OnConnectionRequest(ip, port))
        {
            closesocket(client_sock);
            continue;
        }

        Session* session = new Session;
        session->sessionID = (uint64_t)session + 1;
        session->socket = client_sock;
        memcpy(session->ip, ip, sizeof(ip));
        session->port = port;
        session->count = 0;
        session->sendflag = 0;

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
        map_session[session->sessionID] = session;
        ReleaseSRWLockExclusive(&srw_session);

        OnAccept(session->sessionID, session->ip, session->port);

        recvPost(session);
    }
}

bool LanServer::sendPost(Session* session)
{
    while (true)
    {
        if (InterlockedExchange(&(session->sendflag), 1) == 1)
            return false;   // 이미 누가 보내는 중

        if (session->sendQ.GetUseSize() == 0)
        {
            InterlockedExchange(&(session->sendflag), 0);

            // 깃발 내리는 사이에 다른 스레드가 Enqueue하고
            // sendPost에서 튕겼을 수 있으니 한 번 더 확인
            if (session->sendQ.GetUseSize() > 0)
                continue;

            return false;
        }
        break;
    }

    InterlockedExchange(&(session->sendflag), 1);

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
            InterlockedExchange(&(session->sendflag), 0);
            disconnect(session->sessionID);
            if (InterlockedDecrement(&(session->count)) == 0)
            {
                releaseSession(session);
            }
            return false;
        }
    }
    //printf("count : %d\n", session->count);
    //printf("sendRet : %d\n", sendRet);

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
            disconnect(session->sessionID);
            if (InterlockedDecrement(&(session->count)) == 0)
            {
                releaseSession(session);
            }
            return false;
        }
    }
    //printf("count : %d\n", session->count);
    //printf("recvRet : %d\n", recvRet);

    return true;
}

bool LanServer::releaseSession(Session* session)
{
    AcquireSRWLockExclusive(&srw_session);
    map_session.erase(session->sessionID);
    ReleaseSRWLockExclusive(&srw_session);

    closesocket(session->socket);
    OnRelease(session->sessionID);

    delete session->recvOL;
    delete session->sendOL;
    delete session;

    return true;
}
