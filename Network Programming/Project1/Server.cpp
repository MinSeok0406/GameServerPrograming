#include <iostream>
#include <Windows.h>
#include <list>
#include "Protocol.h"
using namespace std;

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "ws2_32.lib")

#define SERVERPORT 47000

static uint32_t s_id = 0;

SOCKET g_listenSocket;
list<SESSION> g_sessionList;

bool handlePacket(SESSION& session, const char* payload, int len);
bool enqueuePacket(SESSION& session, const char* msg, uint16_t msglen);
bool sendBroadcast(const char* msg);

bool acceptLogic();
bool recvLogic(SESSION& session);
bool sendLogic(SESSION& session);
bool netLogic();

int wmain()
{
    timeBeginPeriod(1);

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 1;
    }

    g_listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (g_listenSocket == INVALID_SOCKET)
    {
        printf("socket : %d\n", WSAGetLastError());
        return 1;
    }

    linger li;
    li.l_onoff = 1;
    li.l_linger = 0;
    auto socketOptRet = setsockopt(g_listenSocket, SOL_SOCKET, SO_LINGER, (const char*)&li, sizeof(li));
    if (socketOptRet == SOCKET_ERROR)
    {
        printf("setsockopt : %d\n", WSAGetLastError());
        return 1;
    }

    SOCKADDR_IN serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);

    auto bindRet = bind(g_listenSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (bindRet == SOCKET_ERROR)
    {
        printf("bind : %d\n", WSAGetLastError());
        return 1;
    }

    u_long on = 1;
    auto ioctRet = ioctlsocket(g_listenSocket, FIONBIO, &on);
    if (ioctRet == SOCKET_ERROR)
    {
        printf("ioctlsocket : %d\n", WSAGetLastError());
        return 1;
    }

    auto listenRet = listen(g_listenSocket, SOMAXCONN);
    if (listenRet == SOCKET_ERROR)
    {
        printf("listen : %d\n", WSAGetLastError());
        return 1;
    }

    while (true)
    {
        netLogic();
    }

    WSACleanup();

    return 0;
}

bool handlePacket(SESSION& session, const char* payload, int len)
{
    char msg[MSG_MAX_LEN + 1];
    memcpy(msg, payload, len);
    msg[len] = '\0';

    sendBroadcast(msg);

    return true;
}

bool enqueuePacket(SESSION& session, const char* msg, uint16_t msglen)
{
    uint32_t needByte = sizeof(HEADER) + msglen;
    if (needByte > BUFSIZE - session.sendByte)
    {
        return false;
    }

    HEADER header;
    header.packetSize = msglen;

    memcpy(session.sendQ + session.sendByte, &header, sizeof(header));
    memcpy(session.sendQ + session.sendByte + sizeof(header), msg, msglen);
    session.sendByte += needByte;

    return true;
}

bool sendBroadcast(const char* msg)
{
    uint16_t len = (uint16_t)strnlen(msg, MSG_MAX_LEN);

    for (auto& session : g_sessionList)
    {
        enqueuePacket(session, msg, len);
    }

    return true;
}

bool acceptLogic()
{
    SOCKADDR_IN clientaddr;
    int addrlen = sizeof(clientaddr);

    SOCKET clientSocket = accept(g_listenSocket, (SOCKADDR*)&clientaddr, &addrlen);
    if (clientSocket == INVALID_SOCKET)
    {
        printf("accept : %d\n", WSAGetLastError());
        return false;
    }

    g_sessionList.emplace_back();
    SESSION& session = g_sessionList.back();
    session.sock = clientSocket;
    session.id = s_id++;
    session.sendByte = 0;
    session.recvByte = 0;

    // 주어진 송수신 실행

    return true;
}

bool recvLogic(SESSION& session)
{
    auto recvRet = recv(session.sock, session.recvQ + session.recvByte, BUFSIZE - session.recvByte, 0);
    if (recvRet == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAEWOULDBLOCK)
        {
            printf("recv : %d\n", WSAGetLastError());
        }

        return false;
    }
    else if (recvRet == 0)
    {
        return true;
    }
    session.recvByte += recvRet;

    int offset = 0;
    while (true)
    {
        int remain = session.recvByte - offset;
        if (remain < sizeof(HEADER))
        {
            break;
        }

        HEADER header;
        memcpy(&header, session.recvQ + offset, sizeof(header));

        if (remain < (sizeof(header) + header.packetSize))
        {
            break;
        }

        handlePacket(session, session.recvQ + offset + sizeof(header), header.packetSize);
        offset += sizeof(header) + header.packetSize;
    }

    if (offset > 0)
    {
        memmove(session.recvQ, session.recvQ + offset, session.recvByte - offset);
        session.recvByte -= offset;
    }

    return true;
}

bool sendLogic(SESSION& session)
{
    auto sendRet = send(session.sock, session.sendQ, session.sendByte, 0);
    if (sendRet == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAEWOULDBLOCK)
        {
            printf("send : %d\n", WSAGetLastError());
        }

        return true;
    }

    memmove(session.sendQ, session.sendQ + sendRet, session.sendByte - sendRet);
    session.sendByte -= sendRet;

    return true;
}

bool netLogic()
{
    fd_set rset;
    fd_set wset;

    FD_ZERO(&rset);
    FD_ZERO(&wset);

    FD_SET(g_listenSocket, &rset);

    for (auto& session : g_sessionList)
    {
        FD_SET(session.sock, &rset);
        if (session.sendByte > 0)
        {
            FD_SET(session.sock, &wset);
        }
    }

    auto selectRet = select(0, &rset, &wset, NULL, NULL);
    if (selectRet == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAEWOULDBLOCK)
        {
            printf("select : %d\n", WSAGetLastError());
        }

        return false;
    }

    if (selectRet > 0)
    {
        if (FD_ISSET(g_listenSocket, &rset))
        {
            acceptLogic();
        }

        for (auto& session : g_sessionList)
        {
            if (FD_ISSET(session.sock, &rset))
            {
                recvLogic(session);
                selectRet--;
            }

            if (FD_ISSET(session.sock, &wset))
            {
                sendLogic(session);
                selectRet--;
            }
        }
    }


    return true;
}