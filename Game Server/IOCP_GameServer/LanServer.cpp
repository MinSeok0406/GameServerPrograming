#include "LanServer.h"
#include <process.h>
#include <conio.h>
#include "CrashDump.h"

CrashDump g_crashDump;
OVERLAPPED g_sendPostOL;

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

    linger optval;
    optval.l_onoff = 1;
    optval.l_linger = 0;
    setsockopt(listen_sock, SOL_SOCKET, SO_LINGER, (const char*)&optval, sizeof(optval));

    int newSize = 0;
    setsockopt(listen_sock, SOL_SOCKET, SO_SNDBUF, (const char*)&newSize, sizeof(newSize));
    setsockopt(listen_sock, SOL_SOCKET, SO_RCVBUF, (const char*)&newSize, sizeof(newSize));

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

    for (auto i = 0; i < CLIENT; ++i)
    {
        CloseHandle(hThreads[i]);
    }

    for (auto& session : sessions)
    {
        if (session == nullptr)
        {
            continue;
        }

        closesocket(session->socket);
        delete session->recvOL;
        delete session->sendOL;
        delete session;
        session = nullptr;
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
    Session* session = sessions[sessionID & INDEX_MASK];
    if (session == nullptr || session->sessionID != sessionID)
    {
        return false;
    }

    CancelIoEx((HANDLE)session->socket, NULL);

    return true;
}

bool LanServer::sendPacket(uint64_t sessionID, SerializationBuffer* packet)
{
    Session* session = sessions[sessionID & INDEX_MASK];
    if (session == nullptr || session->sessionID != sessionID)
    {
        g_crashDump.Crash();
        freePacket(packet);
        return false;
    }

    if (session->sendArrayCount >= ce_SendArraySize)
    {
        g_crashDump.Crash();
        freePacket(packet);
        disconnect(sessionID);
        return false;
    }

    if (session->sendArray[session->sendRear] != nullptr)
    {
        g_crashDump.Crash();
    }

    session->sendArray[session->sendRear] = packet;
    session->sendRear = (session->sendRear + 1) & ce_SendArrayMask;
    InterlockedIncrement(&session->sendArrayCount);

    if (session->sendflag == 0)
    {
        InterlockedIncrement(&session->count);
        PostQueuedCompletionStatus(hcp, 0, (uint64_t)session, &g_sendPostOL);
    }

    return true;
}

SerializationBuffer* LanServer::allocPacket()
{
    SerializationBuffer* packet = new SerializationBuffer;
    packet->clear();
    return packet;
}

void LanServer::freePacket(SerializationBuffer* packet)
{
    delete packet;
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

        if (overlapped == &g_sendPostOL)
        {
            sendPost(session);
        }
        else if (GQCSret == FALSE || transferred == 0)
        {
            // 클라가 RST 보냄 -> 소켓 정리
            // 클라 정상 종료
            disconnect(session->sessionID);
            //printf("Client Exit\n");
        }
        else if (&(session->recvOL->overlapped) == overlapped)
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
            // WSABUF에 걸려있는 버퍼가 없는데, send 완료 통지가 오는게 말이 안됨
            if (session->sendingCount == 0)
            {
                g_crashDump.Crash();
            }

            for (long i = 0; i < session->sendingCount; ++i)
            {
                freePacket(session->sendingPackets[i]);
                session->sendingPackets[i] = nullptr;
            }
            session->sendingCount = 0;

            InterlockedExchange(&(session->sendflag), 0);
            sendPost(session);
        }

        if (InterlockedDecrement(&(session->count)) == 0)
        {
            //printf("Delete session\n");
            //printf("count : %d\n", session->count);
            releaseSession(session);
        }

        //printf("Accept : %d \t Disconnect : %d \t Release : %d\n", sessionCount, disconnectCount, releaseCount);
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

        AcquireSRWLockExclusive(&indexLock);
        if (sessionIndex.empty())
        {
            ReleaseSRWLockExclusive(&indexLock);
            closesocket(client_sock);
            continue;
        }
        uint16_t index = sessionIndex.top();
        sessionIndex.pop();
        ReleaseSRWLockExclusive(&indexLock);

        Session* session = new Session;
        session->sessionID = (++idCounter << 16) | index;
        session->socket = client_sock;
        memcpy(session->ip, ip, sizeof(ip));
        session->port = port;
        session->sendRear = 0;
        session->sendFront = 0;
        session->sendArrayCount = 0;
        session->sendingCount = 0;
        
        for (auto i = 0; i < ce_SendArraySize; ++i)
        {
            session->sendArray[i] = nullptr;
        }

        for (auto i = 0; i < ce_MaxWsaBuf; ++i)
        {
            session->sendingPackets[i] = nullptr;
        }

        session->count = 1;
        session->sendflag = 0;

        session->sendOL = new SendOverlapped;
        memset(&(session->sendOL->overlapped), 0, sizeof(session->sendOL->overlapped));
        session->sendOL->overlappedType = (uint32_t)TYPE::SEND;

        session->recvOL = new RecvOverlapped;
        memset(&(session->recvOL->overlapped), 0, sizeof(session->recvOL->overlapped));
        session->recvOL->overlappedType = (uint32_t)TYPE::RECV;

        CreateIoCompletionPort((HANDLE)session->socket, hcp, (uint64_t)session, 0);

        sessions[index] = session;
        InterlockedIncrement(&sessionCount);

        OnAccept(session->sessionID, session->ip, session->port);

        recvPost(session);

        if (InterlockedDecrement(&(session->count)) == 0)
        {
            //printf("Delete session\n");
            //printf("count : %d\n", session->count);
            releaseSession(session);
        }
    }
}

bool LanServer::sendPost(Session* session)
{
    while (true)
    {
        if (InterlockedExchange(&(session->sendflag), 1) == 1)
            return false;   // 이미 누가 보내는 중

        if (session->sendArrayCount == 0)
        {
            InterlockedExchange(&(session->sendflag), 0);

            // 다른 스레드가 Enqueue하고
            // sendPost에서 튕겼을 수 있으니 한 번 더 확인
            if (session->sendArrayCount > 0)
                continue;

            return false;
        }
        break;
    }

    long cnt = min((int)session->sendArrayCount, ce_MaxWsaBuf);

    WSABUF wsabufs[ce_MaxWsaBuf];
    for (long i = 0; i < cnt; ++i)
    {
        SerializationBuffer* packet = session->sendArray[session->sendFront];
        if (packet == nullptr)
        {
            g_crashDump.Crash();
        }

        session->sendArray[session->sendFront] = nullptr;
        session->sendFront = (session->sendFront + 1) & ce_SendArrayMask;

        session->sendingPackets[i] = packet;
        wsabufs[i].buf = packet->getBufferPtr();
        wsabufs[i].len = packet->getDataSize();
    }
    session->sendingCount = cnt;
    InterlockedExchangeAdd(&session->sendArrayCount, -cnt);

    memset(&(session->sendOL->overlapped), 0, sizeof(session->sendOL->overlapped));
    InterlockedIncrement(&(session->count));
    
    auto sendRet = WSASend(session->socket, wsabufs, cnt, NULL, 0, &(session->sendOL->overlapped), NULL);
    if (sendRet == SOCKET_ERROR)
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
    //printf("sendRet : %d\n", sendRet);

    return true;
}

bool LanServer::recvPost(Session* session)
{
    memset(&(session->recvOL->overlapped), 0, sizeof(session->recvOL->overlapped));
    WSABUF recvWsaBuf;
    recvWsaBuf.buf = session->recvQ.GetRearBufferPtr();
    recvWsaBuf.len = session->recvQ.DirectEnqueueSize();

    if (recvWsaBuf.len == 0)
    {
        g_crashDump.Crash();
    }

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
    uint64_t sessionID = session->sessionID;
    uint16_t index = static_cast<uint16_t>(sessionID & INDEX_MASK);

    for (long i = 0; i < session->sendingCount; ++i)
    {
        freePacket(session->sendingPackets[i]);
    }

    while (session->sendArrayCount > 0)
    {
        freePacket(session->sendArray[session->sendFront]);
        session->sendFront = (session->sendFront + 1) & ce_SendArrayMask;
        --session->sendArrayCount;
    }

    sessions[index] = nullptr;
    closesocket(session->socket);
    OnRelease(session->sessionID);

    delete session->recvOL;
    delete session->sendOL;
    delete session;

    InterlockedDecrement(&sessionCount);

    AcquireSRWLockExclusive(&indexLock);
    sessionIndex.push(index);
    ReleaseSRWLockExclusive(&indexLock);
    return true;
}