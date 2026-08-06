#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <time.h>
#include <list>
#include <algorithm>
#include "Protocol.h"
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

#define SERVERPORT      47000

list<st_USER> g_userList;
SOCKET g_listensocket;
SOCKET g_clientsocket;
SOCKADDR_IN clientaddr;

bool g_shutdown = false;

static int s_id = 0;

bool networkLogic();
bool Update();

// 네트워크 함수 -> accepct, send, recv...
bool netProc_Accept();
bool netProc_Send(st_USER* user);
bool netProc_Recv(st_USER* user);
bool sendPacket_Unicast(st_USER* user, char* header, char* packet, int size);
bool sendPacket_Broadcast(st_USER* user, char* header, char* packet, int size);

int wmain()
{
    timeBeginPeriod(1);
    srand((unsigned int)(time(nullptr)));

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("%d\n", WSAGetLastError());
        return 0;
    }

    g_listensocket = socket(AF_INET, SOCK_STREAM, 0);
    if (g_listensocket == INVALID_SOCKET)
    {
        printf("%d\n", WSAGetLastError());
        return 0;
    }

    SOCKADDR_IN serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    int bindRet = bind(g_listensocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (bindRet == SOCKET_ERROR)
    {
        printf("%d\n", WSAGetLastError());
        return 0;
    }

    int listenRet = listen(g_listensocket, SOMAXCONN);
    if (listenRet == SOCKET_ERROR)
    {
        printf("%d\n", WSAGetLastError());
        return 0;
    }

    u_long on = 1;
    int nonbloRet = ioctlsocket(g_listensocket, FIONBIO, &on);
    if (nonbloRet == SOCKET_ERROR)
    {
        printf("%d\n", WSAGetLastError());
        return 0;
    }

    while (!g_shutdown)
    {
        networkLogic();
        Update();
    }
    
    WSACleanup();

    return 1;
}

bool networkLogic()
{
    fd_set rset;
    fd_set wset;

    FD_ZERO(&rset);
    FD_ZERO(&wset);

    FD_SET(g_listensocket, &rset);

    for (auto& user : g_userList)
    {
        FD_SET(user._sock, &rset);
        if (user._sendQ.GetUseSize() > 0)
        {
            FD_SET(user._sock, &wset);
        }
    }

    int selectRet = select(0, &rset, &wset, 0, NULL);
    if (selectRet == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAEWOULDBLOCK)
        {
            printf("%d\n", WSAGetLastError());
            g_shutdown = true;
        }
        
        return false;
    }

    if (selectRet > 0)
    {
        if (FD_ISSET(g_listensocket, &rset))
        {
            netProc_Accept();
        }

        for (auto& user : g_userList)
        {
            if (FD_ISSET(user._sock, &rset))
            {
                selectRet--;
                netProc_Recv(&user);
            }

            if (FD_ISSET(user._sock, &wset))
            {
                selectRet--;
                netProc_Send(&user);
            }
        }
    }

    return true;
}

bool Update()
{
    


    return true;
}

bool netProc_Accept()
{
    int addrlen = sizeof(clientaddr);
    g_clientsocket = accept(g_listensocket, (SOCKADDR*)&clientaddr, &addrlen);
    if (g_clientsocket == INVALID_SOCKET)
    {
        if (WSAGetLastError() != WSAEWOULDBLOCK)
        {
            printf("%d\n", WSAGetLastError());
        }
        
        return false;
    }

    g_userList.emplace_back();
    st_USER& createuser = g_userList.back();
    createuser._id = s_id++;
    createuser._sock = g_clientsocket;

    // 신규 유저 처리

    return true;
}

bool netProc_Send(st_USER* user)
{
    while (true)
    {
        if (user->_sendQ.GetUseSize() < sizeof(st_HEADER))
        {
            break;
        }

        int sendRet = send(user->_sock, user->_sendQ.GetFrontBufferPtr(),
            user->_sendQ.DirectDequeueSize(), 0);
        if (sendRet == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSAEWOULDBLOCK)
            {
                printf("%d\n", WSAGetLastError());
            }

            break;
        }

        user->_sendQ.MoveFront(sendRet);
    }

    return true;
}

bool netProc_Recv(st_USER* user)
{
    if (user->_recvQ.GetFreeSize() < sizeof(st_HEADER))
    {
        return false;
    }

    int recvRet = recv(user->_sock, user->_recvQ.GetRearBufferPtr(),
        user->_recvQ.DirectEnqueueSize(), 0);
    if (recvRet == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAEWOULDBLOCK)
        {
            printf("%d\n", WSAGetLastError());
        }

        return false;
    }
    else if (recvRet == 0)
    {
        return true;
    }

    user->_recvQ.MoveRear(recvRet);

    while (true)
    {
        if (user->_recvQ.GetUseSize() <= sizeof(st_HEADER))
        {
            return false;
        }

        char buf[2];
        int peekRet = user->_recvQ.Peek(buf, sizeof(st_HEADER));
        if (peekRet != sizeof(st_HEADER))
        {
            __debugbreak();
        }

        st_HEADER* header = (st_HEADER*)buf;
        if (user->_recvQ.GetUseSize() < sizeof(st_HEADER) + header->_packetsize)
        {
            return false;
        }

        user->_recvQ.MoveFront(peekRet);

        char message[500];
        peekRet = user->_recvQ.Peek(message, header->_packetsize);
        if (peekRet != header->_packetsize)
        {
            __debugbreak();
        }
        user->_recvQ.MoveFront(peekRet);

        // 한 유저에게 온 메시지 처리
    }


    return true;
}

bool sendPacket_Unicast(st_USER* user, char* header, char* packet, int size)
{
    if (user->_sendQ.GetFreeSize() < size)
    {
        return false;
    }

    user->_sendQ.Enqueue(header, sizeof(st_HEADER));
    user->_sendQ.Enqueue(packet, size - sizeof(st_HEADER));

    return true;
}

bool sendPacket_Broadcast(st_USER* user, char* header, char* packet, int size)
{
    for (auto& users : g_userList)
    {
        if (users._id != user->_id)
        {
            sendPacket_Unicast(&users, header, packet, size);
        }
    }

    return true;
}
