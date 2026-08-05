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

#define SERVERPORT 5000

list<st_SESSION> g_sessionList;
SOCKET g_listenSocket;
SOCKET g_clientSocket;
SOCKADDR_IN g_clientaddr;
bool g_bShutdown = false;
unsigned int tick;

static int g_id = 0;

// 세션 생성 및 삭제 함수
bool fcreateSession();
bool fdisconnect(st_SESSION* session);

// 네트워크 함수
bool netProc_Accept();
bool netProc_Recv(st_SESSION* session);
bool netProc_Send(st_SESSION* session);
bool sendPacket_Unicast(st_SESSION* session, char* header, char* packet, int size);
bool sendPacket_Broadcast(st_SESSION* session, char* header, char* packet, int size);
bool packetProc(st_SESSION* session, unsigned char packetType, char* packet);

// 컨텐츠 함수
bool netPacketProc_MoveStart(st_SESSION* session, char* packet);
bool netPacketProc_MoveStop(st_SESSION* session, char* packet);
bool netPacketProc_Attack1(st_SESSION* session, char* packet);
bool netPacketProc_Attack2(st_SESSION* session, char* packet);
bool netPacketProc_Attack3(st_SESSION* session, char* packet);

// 전체적인 게임 함수
bool netIOProcess();
bool Update();

// 네트워크 프로토콜 함수
bool npfCreateCharacter(st_HEADER* header, st_SC_CREATE_MY_CHARACTER* packet,
    unsigned int id, unsigned char direction, short x, short y, char hp);
bool npfCreateOtherCharacter(st_HEADER* header, st_SC_CREATE_OTHER_CHARACTER* packet,
    unsigned int id, unsigned char direction, short x, short y, char hp);
bool npfDeleteCharacter(st_HEADER* header, st_SC_DELETE_CHARACTER* packet,
    unsigned int id);
bool npfMoveStart(st_HEADER* header, st_SC_MOVE_START* packet, unsigned int id,
    unsigned char direction, short x, short y);
bool npfMoveStop(st_HEADER* header, st_SC_MOVE_STOP* packet, unsigned int id,
    unsigned char direction, short x, short y);
bool npfAttack1(st_HEADER* header, st_SC_ATTACK1* packet, unsigned int id,
    unsigned char direction, short x, short y);
bool npfAttack2(st_HEADER* header, st_SC_ATTACK2* packet, unsigned int id,
    unsigned char direction, short x, short y);
bool npfAttack3(st_HEADER* header, st_SC_ATTACK3* packet, unsigned int id,
    unsigned char direction, short x, short y);
bool npfDamage(st_HEADER* header, st_SC_DAMAGE* packet, unsigned int attackID,
    unsigned int damageID, char damageHP);


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
    g_sessionList.emplace_back();
    st_SESSION& createSession = g_sessionList.back();
    createSession._socket = g_clientSocket;
    createSession._dwSessionID = g_id++;
    InetNtop(AF_INET, &g_clientaddr.sin_addr, createSession._ip, sizeof(createSession._ip));
    createSession._port = htons(g_clientaddr.sin_port);
    createSession._dwAction = 8;
    createSession._byDirection = dfPACKET_MOVE_DIR_RR;
    createSession._shX = (rand() % 600) + 15;
    createSession._shY = (rand() % 410) + 55;
    createSession._chHP = 100;

    st_HEADER header;
    st_SC_CREATE_MY_CHARACTER packet;
    int len = sizeof(header) + sizeof(packet);

    npfCreateCharacter(&header, &packet, createSession._dwSessionID, createSession._byDirection,
        createSession._shX, createSession._shY, createSession._chHP);

    // 신규 유저에게 위치 전송
    sendPacket_Unicast(&createSession, (char*)&header, (char*)&packet, len);

    st_HEADER otherheader;
    st_SC_CREATE_OTHER_CHARACTER otherpacket;
    int otherlen = sizeof(otherheader) + sizeof(otherpacket);

    // 기존 유저들에게 신규 유저 위치 보내기
    npfCreateOtherCharacter(&otherheader, &otherpacket, createSession._dwSessionID, createSession._byDirection,
        createSession._shX, createSession._shY, createSession._chHP);
    sendPacket_Broadcast(&createSession, (char*)&otherheader, (char*)&otherpacket, otherlen);

    // 신규 유저에게 기존 유저 위치들 전송
    for (auto& session : g_sessionList)
    {
        if (session._dwSessionID != createSession._dwSessionID)
        {
            // 세션 많아지면 개선 필요
            npfCreateOtherCharacter(&otherheader, &otherpacket, session._dwSessionID,
                session._byDirection, session._shX, session._shY, session._chHP);
            sendPacket_Unicast(&createSession, (char*)&otherheader, (char*)&otherpacket, otherlen);
        }
    }

    return true;
}

bool fdisconnect(st_SESSION* session)
{
    closesocket(session->_socket);

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
    if (session->_recvQ.GetFreeSize() < 10)
    {
        return false;
    }

    int recvRet = recv(session->_socket, session->_recvQ.GetRearBufferPtr(),
        session->_recvQ.DirectEnqueueSize(), 0);
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
        session->_chHP = 0;
        return true;
    }

    session->_recvQ.MoveRear(recvRet);

    while (true)
    {
        if (session->_recvQ.GetUseSize() <= sizeof(st_HEADER))
        {
            return false;
        }

        char buf[3];
        int peekRet = session->_recvQ.Peek(buf, sizeof(st_HEADER));
        if (peekRet != sizeof(st_HEADER))
        {
            __debugbreak();
        }

        st_HEADER* header = (st_HEADER*)buf;
        if (header->_byCode != ﻿dfNETWORK_PACKET_CODE)
        {
            session->_recvQ.MoveFront(1);
            continue;
        }

        if (session->_recvQ.GetUseSize() < sizeof(st_HEADER) + header->_bySize)
        {
            return false;
        }

        session->_recvQ.MoveFront(sizeof(st_HEADER));

        char packet[20];
        peekRet = session->_recvQ.Peek(packet, header->_bySize);
        if (peekRet != header->_bySize)
        {
            __debugbreak();
        }
        session->_recvQ.MoveFront(peekRet);

        packetProc(session, header->_byType, packet);
    }

    return true;
}

bool netProc_Send(st_SESSION* session)
{
    while (true)
    {
        // 개선 필요
        if (session->_sendQ.GetUseSize() < sizeof(st_HEADER))
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

bool sendPacket_Unicast(st_SESSION* session, char* header, char* packet, int size)
{
    if (session->_sendQ.GetFreeSize() < size)
    {
        return false;
    }

    session->_sendQ.Enqueue(header, sizeof(st_HEADER));
    session->_sendQ.Enqueue(packet, size - sizeof(st_HEADER));
    return true;
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

bool packetProc(st_SESSION* session, unsigned char packetType, char* packet)
{
    switch (packetType)
    {
    case dfPACKET_CS_MOVE_START:
        netPacketProc_MoveStart(session, packet);
        break;
    case dfPACKET_CS_MOVE_STOP:
        netPacketProc_MoveStop(session, packet);
        break;
    case dfPACKET_CS_ATTACK1:
        netPacketProc_Attack1(session, packet);
        break;
    case dfPACKET_CS_ATTACK2:
        netPacketProc_Attack2(session, packet);
        break;
    case dfPACKET_CS_ATTACK3:
        netPacketProc_Attack3(session, packet);
        break;
    }

    return true;
}

bool netPacketProc_MoveStart(st_SESSION* session, char* packet)
{
    st_CS_MOVE_START* movestart = (st_CS_MOVE_START*)packet;
    if (abs(movestart->_x - session->_shX) > dfERROR_RANGE ||
        abs(movestart->_y - session->_shY) > dfERROR_RANGE)
    {
        session->_chHP = 0;
        printf("Don't move location!!\n");
        return false;
    }

    session->_dwAction = movestart->_direction;

    switch (movestart->_direction)
    {
    case dfPACKET_MOVE_DIR_RR:
    case dfPACKET_MOVE_DIR_RU:
    case dfPACKET_MOVE_DIR_RD:
        session->_byDirection = dfPACKET_MOVE_DIR_RR;
        break;
    case dfPACKET_MOVE_DIR_LL:
    case dfPACKET_MOVE_DIR_LU:
    case dfPACKET_MOVE_DIR_LD:
        session->_byDirection = dfPACKET_MOVE_DIR_LL;
        break;
    }
    session->_shX = movestart->_x;
    session->_shY = movestart->_y;

    st_HEADER header;
    st_SC_MOVE_START sendPacket;
    int len = sizeof(header) + sizeof(sendPacket);
    npfMoveStart(&header, &sendPacket, session->_dwSessionID, movestart->_direction,
        session->_shX, session->_shY);
    sendPacket_Broadcast(session, (char*)&header, (char*)&sendPacket, len);

    return true;
}

bool netPacketProc_MoveStop(st_SESSION* session, char* packet)
{
    st_CS_MOVE_STOP* movestop = (st_CS_MOVE_STOP*)packet;
    if (abs(movestop->_x - session->_shX) > dfERROR_RANGE ||
        abs(movestop->_y - session->_shY) > dfERROR_RANGE)
    {
        session->_chHP;
        printf("Don't move location!!\n");
        return false;
    }

    session->_dwAction = dfPACKET_CS_MOVE_STOP;

    switch (movestop->_direction)
    {
    case dfPACKET_MOVE_DIR_RR:
    case dfPACKET_MOVE_DIR_RU:
    case dfPACKET_MOVE_DIR_RD:
        session->_byDirection = dfPACKET_MOVE_DIR_RR;
        break;
    case dfPACKET_MOVE_DIR_LL:
    case dfPACKET_MOVE_DIR_LU:
    case dfPACKET_MOVE_DIR_LD:
        session->_byDirection = dfPACKET_MOVE_DIR_LL;
        break;
    }
    session->_shX = movestop->_x;
    session->_shY = movestop->_y;

    st_HEADER header;
    st_SC_MOVE_STOP sendPacket;
    int len = sizeof(header) + sizeof(sendPacket);
    npfMoveStop(&header, &sendPacket, session->_dwSessionID, session->_byDirection,
        session->_shX, session->_shY);
    sendPacket_Broadcast(session, (char*)&header, (char*)&sendPacket, len);

    return true;
}

bool netPacketProc_Attack1(st_SESSION* session, char* packet)
{
    // 개선 필요
    // 공격 쿨타임
    st_CS_ATTACK1* attacker = (st_CS_ATTACK1*)packet;
    if (attacker->_direction != session->_byDirection ||
        attacker->_x != session->_shX || attacker->_y != session->_shY)
    {
        session->_byDirection = attacker->_direction;
        session->_shX = attacker->_x;
        session->_shY = attacker->_y;
    }

    st_HEADER header;
    st_SC_ATTACK1 sendPacket;
    int len = sizeof(header) + sizeof(sendPacket);
    npfAttack1(&header, &sendPacket, session->_dwSessionID, session->_byDirection,
        session->_shX, session->_shY);
    sendPacket_Broadcast(session, (char*)&header, (char*)&sendPacket, len);

    for (auto& sessions : g_sessionList)
    {
        if (sessions._dwSessionID != session->_dwSessionID)
        {
            if (attacker->_direction == dfPACKET_MOVE_DIR_LL)
            {
                int minX = attacker->_x - 80;
                int maxX = attacker->_x;
                int minY = attacker->_y - 10;
                int maxY = attacker->_y + 10;
                if (sessions._shX >= minX && sessions._shX <= maxX &&
                    sessions._shY >= minY && sessions._shY <= maxY)
                {
                    st_HEADER header;
                    st_SC_DAMAGE sendPacket;
                    int len = sizeof(header) + sizeof(sendPacket);
                    sessions._chHP -= 1;
                    npfDamage(&header, &sendPacket, session->_dwSessionID,
                        sessions._dwSessionID, sessions._chHP);
                    sendPacket_Unicast(session, (char*)&header, (char*)&sendPacket, len);
                    sendPacket_Broadcast(session, (char*)&header, (char*)&sendPacket, len);
                }
            }
            else if (attacker->_direction == dfPACKET_MOVE_DIR_RR)
            {
                int maxX = attacker->_x + 80;
                int minX = attacker->_x;
                int minY = attacker->_y - 10;
                int maxY = attacker->_y + 10;
                if (sessions._shX >= minX && sessions._shX <= maxX &&
                    sessions._shY >= minY && sessions._shY <= maxY)
                {
                    st_HEADER header;
                    st_SC_DAMAGE sendPacket;
                    int len = sizeof(header) + sizeof(sendPacket);
                    sessions._chHP -= 1;
                    npfDamage(&header, &sendPacket, session->_dwSessionID,
                        sessions._dwSessionID, sessions._chHP);
                    sendPacket_Unicast(session, (char*)&header, (char*)&sendPacket, len);
                    sendPacket_Broadcast(session, (char*)&header, (char*)&sendPacket, len);
                }
            }
        }
    }
    
    return true;
}

bool netPacketProc_Attack2(st_SESSION* session, char* packet)
{
    // 개선 필요
    // 공격 쿨타임
    st_CS_ATTACK2* attacker = (st_CS_ATTACK2*)packet;
    if (attacker->_direction != session->_byDirection ||
        attacker->_x != session->_shX || attacker->_y != session->_shY)
    {
        session->_byDirection = attacker->_direction;
        session->_shX = attacker->_x;
        session->_shY = attacker->_y;
    }

    st_HEADER header;
    st_SC_ATTACK2 sendPacket;
    int len = sizeof(header) + sizeof(sendPacket);
    npfAttack2(&header, &sendPacket, session->_dwSessionID, session->_byDirection,
        session->_shX, session->_shY);
    sendPacket_Broadcast(session, (char*)&header, (char*)&sendPacket, len);

    for (auto& sessions : g_sessionList)
    {
        if (sessions._dwSessionID != session->_dwSessionID)
        {
            if (attacker->_direction == dfPACKET_MOVE_DIR_LL)
            {
                int minX = attacker->_x - 90;
                int maxX = attacker->_x;
                int minY = attacker->_y - 10;
                int maxY = attacker->_y + 10;
                if (sessions._shX >= minX && sessions._shX <= maxX &&
                    sessions._shY >= minY && sessions._shY <= maxY)
                {
                    st_HEADER header;
                    st_SC_DAMAGE sendPacket;
                    int len = sizeof(header) + sizeof(sendPacket);
                    sessions._chHP -= 1;
                    npfDamage(&header, &sendPacket, session->_dwSessionID,
                        sessions._dwSessionID, sessions._chHP);
                    sendPacket_Unicast(session, (char*)&header, (char*)&sendPacket, len);
                    sendPacket_Broadcast(session, (char*)&header, (char*)&sendPacket, len);
                }
            }
            else if (attacker->_direction == dfPACKET_MOVE_DIR_RR)
            {
                int maxX = attacker->_x + 90;
                int minX = attacker->_x;
                int minY = attacker->_y - 10;
                int maxY = attacker->_y + 10;
                if (sessions._shX >= minX && sessions._shX <= maxX &&
                    sessions._shY >= minY && sessions._shY <= maxY)
                {
                    st_HEADER header;
                    st_SC_DAMAGE sendPacket;
                    int len = sizeof(header) + sizeof(sendPacket);
                    sessions._chHP -= 1;
                    npfDamage(&header, &sendPacket, session->_dwSessionID,
                        sessions._dwSessionID, sessions._chHP);
                    sendPacket_Unicast(session, (char*)&header, (char*)&sendPacket, len);
                    sendPacket_Broadcast(session, (char*)&header, (char*)&sendPacket, len);
                }
            }
        }
    }

    return true;
}

bool netPacketProc_Attack3(st_SESSION* session, char* packet)
{
    // 개선 필요
    // 공격 쿨타임
    st_CS_ATTACK3* attacker = (st_CS_ATTACK3*)packet;
    if (attacker->_direction != session->_byDirection ||
        attacker->_x != session->_shX || attacker->_y != session->_shY)
    {
        session->_byDirection = attacker->_direction;
        session->_shX = attacker->_x;
        session->_shY = attacker->_y;
    }

    st_HEADER header;
    st_SC_ATTACK3 sendPacket;
    int len = sizeof(header) + sizeof(sendPacket);
    npfAttack3(&header, &sendPacket, session->_dwSessionID, session->_byDirection,
        session->_shX, session->_shY);
    sendPacket_Broadcast(session, (char*)&header, (char*)&sendPacket, len);

    for (auto& sessions : g_sessionList)
    {
        if (sessions._dwSessionID != session->_dwSessionID)
        {
            if (attacker->_direction == dfPACKET_MOVE_DIR_LL)
            {
                int minX = attacker->_x - 100;
                int maxX = attacker->_x;
                int minY = attacker->_y - 20;
                int maxY = attacker->_y + 20;
                if (sessions._shX >= minX && sessions._shX <= maxX &&
                    sessions._shY >= minY && sessions._shY <= maxY)
                {
                    st_HEADER header;
                    st_SC_DAMAGE sendPacket;
                    int len = sizeof(header) + sizeof(sendPacket);
                    sessions._chHP -= 1;
                    npfDamage(&header, &sendPacket, session->_dwSessionID,
                        sessions._dwSessionID, sessions._chHP);
                    sendPacket_Unicast(session, (char*)&header, (char*)&sendPacket, len);
                    sendPacket_Broadcast(session, (char*)&header, (char*)&sendPacket, len);
                }
            }
            else if (attacker->_direction == dfPACKET_MOVE_DIR_RR)
            {
                int maxX = attacker->_x + 100;
                int minX = attacker->_x;
                int minY = attacker->_y - 20;
                int maxY = attacker->_y + 20;
                if (sessions._shX >= minX && sessions._shX <= maxX &&
                    sessions._shY >= minY && sessions._shY <= maxY)
                {
                    st_HEADER header;
                    st_SC_DAMAGE sendPacket;
                    int len = sizeof(header) + sizeof(sendPacket);
                    sessions._chHP -= 1;
                    npfDamage(&header, &sendPacket, session->_dwSessionID,
                        sessions._dwSessionID, sessions._chHP);
                    sendPacket_Unicast(session, (char*)&header, (char*)&sendPacket, len);
                    sendPacket_Broadcast(session, (char*)&header, (char*)&sendPacket, len);
                }
            }
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
    for (auto iter = g_sessionList.begin(); iter != g_sessionList.end();)
    {
        if (iter->_chHP <= 0)
        {
            // Character Delete 프로토콜 함수 브로드캐스팅
            st_HEADER header;
            st_SC_DELETE_CHARACTER sendPacket;
            int len = sizeof(header) + sizeof(sendPacket);
            npfDeleteCharacter(&header, &sendPacket, iter->_dwSessionID);
            sendPacket_Broadcast(&(*iter), (char*)&header, (char*)&sendPacket, len);

            fdisconnect(&(*iter));
            iter = g_sessionList.erase(iter);
        }
        else
        {
            switch (iter->_dwAction)
            {
            case dfPACKET_MOVE_DIR_LL:
                if (iter->_shX - 3 <= dfRANGE_MOVE_LEFT)
                {
                    break;
                }
                iter->_shX -= 3;
                printf("ID : %d  Direction : %c  X : %d  Y : %d\n", iter->_dwSessionID,
                    iter->_byDirection, iter->_shX, iter->_shY);
                break;
            case dfPACKET_MOVE_DIR_LU:
                if (iter->_shX - 3 <= dfRANGE_MOVE_LEFT 
                    || iter->_shY - 2 <= dfRANGE_MOVE_TOP)
                {
                    break;
                }
                iter->_shX -= 3;
                iter->_shY -= 2;
                printf("ID : %d  Direction : %c  X : %d  Y : %d\n", iter->_dwSessionID,
                    iter->_byDirection, iter->_shX, iter->_shY);
                break;
            case dfPACKET_MOVE_DIR_UU:
                if (iter->_shY - 2 <= dfRANGE_MOVE_TOP)
                {
                    break;
                }
                iter->_shY -= 2;
                printf("ID : %d  Direction : %c  X : %d  Y : %d\n", iter->_dwSessionID,
                    iter->_byDirection, iter->_shX, iter->_shY);
                break;
            case dfPACKET_MOVE_DIR_RU:
                if (iter->_shX + 3 >= dfRANGE_MOVE_RIGHT
                    || iter->_shY - 2 <= dfRANGE_MOVE_TOP)
                {
                    break;
                }
                iter->_shX += 3;
                iter->_shY -= 2;
                printf("ID : %d  Direction : %c  X : %d  Y : %d\n", iter->_dwSessionID,
                    iter->_byDirection, iter->_shX, iter->_shY);
                break;
            case dfPACKET_MOVE_DIR_RR:
                if (iter->_shX + 3 >= dfRANGE_MOVE_RIGHT)
                {
                    break;
                }
                iter->_shX += 3;
                printf("ID : %d  Direction : %c  X : %d  Y : %d\n", iter->_dwSessionID,
                    iter->_byDirection, iter->_shX, iter->_shY);
                break;
            case dfPACKET_MOVE_DIR_RD:
                if (iter->_shX + 3 >= dfRANGE_MOVE_RIGHT
                    || iter->_shY + 2 >= dfRANGE_MOVE_BOTTOM)
                {
                    break;
                }
                iter->_shX += 3;
                iter->_shY += 2;
                printf("ID : %d  Direction : %c  X : %d  Y : %d\n", iter->_dwSessionID,
                    iter->_byDirection, iter->_shX, iter->_shY);
                break;
            case dfPACKET_MOVE_DIR_DD:
                if (iter->_shY + 2 >= dfRANGE_MOVE_BOTTOM)
                {
                    break;
                }
                iter->_shY += 2;
                printf("ID : %d  Direction : %c  X : %d  Y : %d\n", iter->_dwSessionID,
                    iter->_byDirection, iter->_shX, iter->_shY);
                break;
            case dfPACKET_MOVE_DIR_LD:
                if (iter->_shX - 3 <= dfRANGE_MOVE_LEFT
                    || iter->_shY + 2 >= dfRANGE_MOVE_BOTTOM)
                {
                    break;
                }
                iter->_shX -= 3;
                iter->_shY += 2;
                printf("ID : %d  Direction : %c  X : %d  Y : %d\n", iter->_dwSessionID,
                    iter->_byDirection, iter->_shX, iter->_shY);
                break;
            }

            ++iter;
        }
    }

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
    header->_byCode = ﻿dfNETWORK_PACKET_CODE;
    header->_bySize = sizeof(st_SC_CREATE_MY_CHARACTER);
    header->_byType = dfPACKET_SC_CREATE_MY_CHARACTER;

    packet->_id = id;
    packet->_direction = direction;
    packet->_x = x;
    packet->_y = y;
    packet->_hp = hp;

    return true;
}

bool npfCreateOtherCharacter(st_HEADER* header, st_SC_CREATE_OTHER_CHARACTER* packet, unsigned int id, unsigned char direction, short x, short y, char hp)
{
    header->_byCode = ﻿dfNETWORK_PACKET_CODE;
    header->_bySize = sizeof(st_SC_CREATE_OTHER_CHARACTER);
    header->_byType = dfPACKET_SC_CREATE_OTHER_CHARACTER;

    packet->_id = id;
    packet->_direction = direction;
    packet->_x = x;
    packet->_y = y;
    packet->_hp = hp;

    return true;
}

bool npfDeleteCharacter(st_HEADER* header, st_SC_DELETE_CHARACTER* packet, unsigned int id)
{
    header->_byCode = ﻿dfNETWORK_PACKET_CODE;
    header->_bySize = sizeof(st_SC_DELETE_CHARACTER);
    header->_byType = dfPACKET_SC_DELETE_CHARACTER;

    packet->_id = id;

    return true;
}

bool npfMoveStart(st_HEADER* header, st_SC_MOVE_START* packet, unsigned int id, unsigned char direction, short x, short y)
{
    header->_byCode = ﻿dfNETWORK_PACKET_CODE;
    header->_bySize = sizeof(st_SC_MOVE_START);
    header->_byType = dfPACKET_SC_MOVE_START;

    packet->_id = id;
    packet->_direction = direction;
    packet->_x = x;
    packet->_y = y;

    return true;
}

bool npfMoveStop(st_HEADER* header, st_SC_MOVE_STOP* packet, unsigned int id, unsigned char direction, short x, short y)
{
    header->_byCode = ﻿dfNETWORK_PACKET_CODE;
    header->_bySize = sizeof(st_SC_MOVE_STOP);
    header->_byType = dfPACKET_SC_MOVE_STOP;

    packet->_id = id;
    packet->_direction = direction;
    packet->_x = x;
    packet->_y = y;

    return true;
}

bool npfAttack1(st_HEADER* header, st_SC_ATTACK1* packet, unsigned int id, unsigned char direction, short x, short y)
{
    header->_byCode = ﻿dfNETWORK_PACKET_CODE;
    header->_bySize = sizeof(st_SC_ATTACK1);
    header->_byType = dfPACKET_SC_ATTACK1;

    packet->_id = id;
    packet->_direction = direction;
    packet->_x = x;
    packet->_y = y;

    return true;
}

bool npfAttack2(st_HEADER* header, st_SC_ATTACK2* packet, unsigned int id, unsigned char direction, short x, short y)
{
    header->_byCode = ﻿dfNETWORK_PACKET_CODE;
    header->_bySize = sizeof(st_SC_ATTACK2);
    header->_byType = dfPACKET_SC_ATTACK2;

    packet->_id = id;
    packet->_direction = direction;
    packet->_x = x;
    packet->_y = y;

    return true;
}

bool npfAttack3(st_HEADER* header, st_SC_ATTACK3* packet, unsigned int id, unsigned char direction, short x, short y)
{
    header->_byCode = ﻿dfNETWORK_PACKET_CODE;
    header->_bySize = sizeof(st_SC_ATTACK3);
    header->_byType = dfPACKET_SC_ATTACK3;

    packet->_id = id;
    packet->_direction = direction;
    packet->_x = x;
    packet->_y = y;

    return true;
}

bool npfDamage(st_HEADER* header, st_SC_DAMAGE* packet, unsigned int attackID, unsigned int damageID, char damageHP)
{
    header->_byCode = ﻿dfNETWORK_PACKET_CODE;
    header->_bySize = sizeof(st_SC_DAMAGE);
    header->_byType = dfPACKET_SC_DAMAGE;

    packet->_attackID = attackID;
    packet->_damageID = damageID;
    packet->_damageHP = damageHP;

    return true;
}