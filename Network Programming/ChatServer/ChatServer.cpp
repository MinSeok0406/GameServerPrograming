#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <time.h>
#include <list>
#include <string>
#include <algorithm>
#include "Protocol.h"
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

#define SERVERPORT      47000

list<USER> g_userList;
SOCKET g_listensocket;
SOCKET g_clientsocket;
SOCKADDR_IN clientaddr;

bool g_shutdown = false;

static int s_id = 0;

bool networkLogic();
bool Update();

// 네트워크 함수 -> accepct, send, recv...
bool netProc_Accept();
bool netProc_Send(USER* user);
bool netProc_Recv(USER* user);
bool packetProc(USER* user, unsigned char type, char* packet);
bool sendPacket_Unicast(USER* user, char* header, char* packet, int size);
bool sendPacket_Broadcast(USER* user, char* header, char* packet, int size);

// 네트워크 패킷 함수
bool netPacketProc_MSG(USER* user, char* packet);

// 네트워크 프로토콜 함수
bool npf_SC_CREATE_USER(HEADER* header, SC_CREATE_USER* packet,
    unsigned int id, char name[20], int nameSize);
bool npf_SC_OTHER_USER(HEADER* header, SC_OTHER_USER* packet,
    unsigned int id, char name[20], int nameSize);
bool npf_SC_MSG(HEADER* header, SC_MSG* packet, unsigned char len, char* msg);

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
    USER& createuser = g_userList.back();
    createuser._id = s_id;
    createuser._sock = g_clientsocket;
    string name = "User" + to_string(s_id);
    memcpy(createuser._name, name.c_str(), sizeof(name.c_str()));
    InetNtop(AF_INET, &clientaddr.sin_addr, createuser._ip, sizeof(createuser._ip));
    createuser._port = ntohs(clientaddr.sin_port);
    
    // 신규 유저 정보 전송
    HEADER header;
    SC_CREATE_USER sendPacket;
    int packetSize = sizeof(header) + sizeof(sendPacket);
    npf_SC_CREATE_USER(&header, &sendPacket, createuser._id, createuser._name,
        sizeof(createuser._name));
    sendPacket_Unicast(&createuser, (char*)&header, (char*)&sendPacket, packetSize);

    
    HEADER otherheader;
    SC_OTHER_USER otherPacket;
    int size = sizeof(otherheader) + sizeof(otherPacket);

    // 기존 유저들에게 신규 유저 정보 전송
    npf_SC_OTHER_USER(&header, &otherPacket, createuser._id, createuser._name, sizeof(createuser._name));
    sendPacket_Broadcast(&createuser, (char*)&header, (char*)&sendPacket, packetSize);

    // 신규 유저에게 기존 유저들 정보 전송
    for (auto& users : g_userList)
    {
        if (users._id != createuser._id)
        {
            npf_SC_OTHER_USER(&header, &otherPacket, users._id, users._name, sizeof(users._name));
            sendPacket_Unicast(&createuser, (char*)&otherheader, (char*)&otherPacket, size);
        }
    }

    return true;
}

bool netProc_Send(USER* user)
{
    while (true)
    {
        if (user->_sendQ.GetUseSize() < sizeof(HEADER))
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

bool netProc_Recv(USER* user)
{
    if (user->_recvQ.GetFreeSize() < sizeof(HEADER))
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
        if (user->_recvQ.GetUseSize() <= sizeof(HEADER))
        {
            return false;
        }

        char buf[2];
        int peekRet = user->_recvQ.Peek(buf, sizeof(HEADER));
        if (peekRet != sizeof(HEADER))
        {
            __debugbreak();
        }

        HEADER* header = (HEADER*)buf;
        if (user->_recvQ.GetUseSize() < sizeof(HEADER) + header->_packetsize)
        {
            return false;
        }

        user->_recvQ.MoveFront(peekRet);

        char packet[250];
        peekRet = user->_recvQ.Peek(packet, header->_packetsize);
        if (peekRet != header->_packetsize)
        {
            __debugbreak();
        }
        user->_recvQ.MoveFront(peekRet);

        packetProc(user, header->_type, packet);
    }

    return true;
}

bool packetProc(USER* user, unsigned char type, char* packet)
{
    switch (type)
    {
    case PACKET_CS_MSG:
        netPacketProc_MSG(user, packet);
        break;
    }

    return true;
}

bool sendPacket_Unicast(USER* user, char* header, char* packet, int size)
{
    if (user->_sendQ.GetFreeSize() < size)
    {
        return false;
    }

    user->_sendQ.Enqueue(header, sizeof(HEADER));
    user->_sendQ.Enqueue(packet, size - sizeof(HEADER));

    return true;
}

bool sendPacket_Broadcast(USER* user, char* header, char* packet, int size)
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

bool netPacketProc_MSG(USER* user, char* packet)
{
    CS_MSG* msgpacket = (CS_MSG*)packet;
    
    // 추가할 예정있다면 작성
    
    HEADER header;
    SC_MSG sendPacket;
    int size = sizeof(header) + sizeof(sendPacket);
    npf_SC_MSG(&header, &sendPacket, msgpacket->_len, msgpacket->_msg);
    sendPacket_Broadcast(user, (char*)&header, (char*)&sendPacket, size);

    return true;
}

bool npf_SC_CREATE_USER(HEADER* header, SC_CREATE_USER* packet, unsigned int id, char name[20], int nameSize)
{
    header->_packetsize = sizeof(SC_CREATE_USER);
    header->_type = PACKET_SC_CREATE_USER;

    packet->_id = id;
    memcpy(packet->_name, name, nameSize);

    return true;
}

bool npf_SC_OTHER_USER(HEADER* header, SC_OTHER_USER* packet, unsigned int id, char name[20], int nameSize)
{
    header->_packetsize = sizeof(SC_OTHER_USER);
    header->_type = PACKET_SC_OTHER_USER;

    packet->_id = id;
    memcpy(packet->_name, name, nameSize);

    return true;
}

bool npf_SC_MSG(HEADER* header, SC_MSG* packet, unsigned char len, char* msg)
{
    header->_packetsize = sizeof(SC_MSG);
    header->_type = PACKET_SC_MSG;

    packet->_len = len;
    memcpy(packet->_msg, msg, len);

    return false;
}
