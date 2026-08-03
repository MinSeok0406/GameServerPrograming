#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <time.h>
#include <list>
#include <string>
#include "Protocol.h"
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

#define SERVERPORT 5000

list<st_SESSION> g_sessionList;
SOCKET g_listenSocket;
SOCKET g_clientSocket;
SOCKADDR_IN g_clientaddr;
bool g_bShutdown = false;
unsigned int tick;

static int g_id = 0;

// 세션 생성 함수
bool fcreateSession();

// 네트워크 함수
bool netProc_Accept();
bool netProc_Recv(st_SESSION* session);
bool netProc_Send(st_SESSION* session);
bool sendPacket_Unicast(st_SESSION* session, char* header, char* packet, int size);
bool sendPacket_Broadcast(st_SESSION* session, char* header, char* packet, int size);

// 전체적인 게임 함수
bool netIOProcess();
bool Update();

// 네트워크 프로토콜 함수
bool npfCreateCharacter(st_HEADER* header, st_SC_CREATE_MY_CHARACTER* packet,
    unsigned int id, unsigned char direction, short x, short y, char hp);

int wmain(int argc, WCHAR* argv[])
{
    timeBeginPeriod(1);
    srand(unsigned int(time(nullptr)));

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 1;
    }

    g_listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (g_listenSocket == INVALID_SOCKET)
    {
        printf("%d\n", WSAGetLastError());
        return 1;
    }

    SOCKADDR_IN serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    int bindRet = bind(g_listenSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (bindRet == SOCKET_ERROR)
    {
        printf("%d\n", WSAGetLastError());
        return 1;
    }

    u_long on = 1;
    int nbioret = ioctlsocket(g_listenSocket, FIONBIO, &on);
    if (nbioret == SOCKET_ERROR)
    {
        printf("%d\n", WSAGetLastError());
        return 1;
    }

    int listenRet = listen(g_listenSocket, SOMAXCONN);
    if (listenRet == SOCKET_ERROR)
    {
        printf("%d\n", WSAGetLastError());
        return 1;
    }

    tick = timeGetTime();
    while (!g_bShutdown)
    {
        netIOProcess();
        Update();
    }

    closesocket(g_listenSocket);
    WSACleanup();

    return 1;
}

bool fcreateSession()
{
    st_SESSION createSession;
    createSession._socket = g_clientSocket;
    createSession._dwSessionID = g_id++;
    InetNtop(AF_INET, &g_clientaddr.sin_addr, createSession._ip, sizeof(createSession._ip));
    createSession._port = htons(g_clientaddr.sin_port);
    createSession._dwAction = 0;
    createSession._byDirection = 0;
    createSession._shX = (rand() % 600) + 15;
    createSession._shY = (rand() % 410) + 55;
    createSession._chHP = 100;
    g_sessionList.push_back(createSession);

    st_HEADER header;
    st_SC_CREATE_MY_CHARACTER packet;
    int len = sizeof(header) + sizeof(packet);

    npfCreateCharacter(&header, &packet, createSession._dwSessionID, createSession._byDirection,
        createSession._shX, createSession._shY, createSession._chHP);
    sendPacket_Unicast(&createSession, (char*)&header, (char*)&packet, len);

    return true;
}

bool netProc_Accept()
{
    int addrlen = sizeof(g_clientaddr);
    g_clientSocket = accept(g_listenSocket, (SOCKADDR*)&g_clientaddr, &addrlen);
    if (g_clientSocket == INVALID_SOCKET)
    {
        printf("%d\n", WSAGetLastError());
        return false;
    }

    fcreateSession();

    return true;
}

bool netProc_Recv(st_SESSION* session)
{



    return true;
}

bool netProc_Send(st_SESSION* session)
{



    return true;
}

bool sendPacket_Unicast(st_SESSION* session, char* header, char* packet, int size)
{
    if (session->_sendQ.GetFreeSize() < size)
    {
        return false;
    }

    session->_sendQ.Enqueue(header, 3);
    session->_sendQ.Enqueue(packet, size - 3);

    while (true)
    {
        if (session->_sendQ.GetUseSize() < size)
        {
            return true;
        }

        int sendRet = send(session->_socket, session->_sendQ.GetFrontBufferPtr(),
            session->_sendQ.DirectDequeueSize(), 0);
        if (sendRet == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSAEWOULDBLOCK)
            {
                printf("%d\n", WSAGetLastError());
            }
            return true;
        }

        session->_sendQ.MoveFront(sendRet);
    }
}

bool sendPacket_Broadcast(st_SESSION* session, char* header, char* packet, int size)
{
    for (auto& sessions : g_sessionList)
    {
        if (sessions._dwSessionID != session->_dwSessionID)
        {
            sendPacket_Unicast(&sessions, header, packet, size);
        }
    }

    return true;
}

bool netIOProcess()
{
    FD_SET readSet;
    FD_SET writeSet;

    FD_ZERO(&readSet);
    FD_ZERO(&writeSet);

    FD_SET(g_listenSocket, &readSet);

    for (auto iter = g_sessionList.begin(); iter != g_sessionList.end(); ++iter)
    {
        FD_SET(iter->_socket, &readSet);
        if (iter->_sendQ.GetUseSize() > 0)
        {
            FD_SET(iter->_socket, &writeSet);
        }
    }

    timeval t;
    int selectRet;
    int cnt = 0;
    bool bProcFlag = false;

    t.tv_sec = 0;
    t.tv_usec = 0;
    selectRet = select(0, &readSet, &writeSet, 0, &t);
    if (selectRet == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAEWOULDBLOCK)
        {
            printf("%d\n", WSAGetLastError());
            g_bShutdown = true;
        }

        return false;
    }

    if (selectRet > 0)
    {
        if (FD_ISSET(g_listenSocket, &readSet))
        {
            netProc_Accept();
        }

        for (auto& session : g_sessionList)
        {
            if (FD_ISSET(session._socket, &readSet))
            {
                selectRet--;
                netProc_Recv(&session);
            }

            if (FD_ISSET(session._socket, &writeSet))
            {
                selectRet--;
                netProc_Send(&session);
            }
        }
    }

    return true;
}

bool Update()
{
    // TODO


    int useTime = (int)(timeGetTime() - tick);
    if (useTime < 20)
    {
        Sleep(20 - useTime);
    }
    else if (useTime > 100)
    {
        useTime = timeGetTime();
    }
    tick += 20;

    return true;
}

bool npfCreateCharacter(st_HEADER* header, st_SC_CREATE_MY_CHARACTER* packet, unsigned int id, unsigned char direction, short x, short y, char hp)
{



    return true;
}
