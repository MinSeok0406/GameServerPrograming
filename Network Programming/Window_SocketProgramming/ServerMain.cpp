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

unsigned long g_updateTime = timeGetTime();
const float c_move_speed_X = 150.0f;
const float c_move_speed_Y = 100.0f;
const float c_move_speed_DIAG_X = 150.0f * 0.8321f;
const float c_move_speed_DIAG_Y = 150.0f * 0.8321f;

static int g_id = 0;

// 세션 생성 및 삭제 함수
bool fcreateSession();
bool fdisconnect(st_SESSION* session);

// 네트워크 함수
bool netProc_Accept();
bool netProc_Recv(st_SESSION* session);
bool netProc_Send(st_SESSION* session);
bool sendPacket_Unicast(st_SESSION* session, SerializationBuffer* packet);
bool sendPacket_Broadcast(st_SESSION* session, SerializationBuffer* packet);
bool packetProc(st_SESSION* session, unsigned char packetType, SerializationBuffer* packet);

// 컨텐츠 함수
bool netPacketProc_MoveStart(st_SESSION* session, SerializationBuffer* packet);
bool netPacketProc_MoveStop(st_SESSION* session, SerializationBuffer* packet);
bool netPacketProc_Attack1(st_SESSION* session, SerializationBuffer* packet);
bool netPacketProc_Attack2(st_SESSION* session, SerializationBuffer* packet);
bool netPacketProc_Attack3(st_SESSION* session, SerializationBuffer* packet);

// 전체적인 게임 함수
bool netIOProcess();
bool Update();

// 네트워크 프로토콜 함수
bool npfCreateCharacter(SerializationBuffer* packet, unsigned int id,
    unsigned char direction, short x, short y, char hp);
bool npfCreateOtherCharacter(SerializationBuffer* packet, unsigned int id,
    unsigned char direction, short x, short y, char hp);
bool npfDeleteCharacter(SerializationBuffer* packet, unsigned int id);
bool npfMoveStart(SerializationBuffer* packet, unsigned int id, unsigned char direction,
    short x, short y);
bool npfMoveStop(SerializationBuffer* packet, unsigned int id, unsigned char direction,
    short x, short y);
bool npfAttack1(SerializationBuffer* packet, unsigned int id, unsigned char direction,
    short x, short y);
bool npfAttack2(SerializationBuffer* packet, unsigned int id, unsigned char direction,
    short x, short y);
bool npfAttack3(SerializationBuffer* packet, unsigned int id, unsigned char direction,
    short x, short y);
bool npfDamage(SerializationBuffer* packet, unsigned int attackID, unsigned int damageID,
    char damageHP);


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

        // 컨텐츠만 프레임 관리
        // 네트워크 바쁜 대기로 인해서 CPU를 8~10% 잡아 먹지만
        // 그만큼 네트워크 반응성이 좋아서 게임이 부드럽게 진행되는 것 같음
        /*int useTime = (int)(timeGetTime() - tick);
        if (useTime >= 20)
        {
            Update();
            tick += 20;
        }*/

        // CPU가 거의 0에 수렴하도록 소스를 먹지 않는다
        int useTime = (int)(timeGetTime() - tick);
        if (useTime < 20)
        {
            Sleep(20 - useTime);
        }
        else if (useTime > 100)
        {
            tick = timeGetTime();
        }
        tick += 20;
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
    createSession._port = ntohs(g_clientaddr.sin_port);
    createSession._dwAction = 8;
    createSession._byDirection = dfPACKET_MOVE_DIR_RR;
    createSession._shX = (rand() % 600) + 15;
    createSession._shY = (rand() % 410) + 55;
    createSession._chHP = 100;

    SerializationBuffer packet;

    // 신규 유저에게 위치 전송
    npfCreateCharacter(&packet, createSession._dwSessionID, createSession._byDirection,
        createSession._shX, createSession._shY, createSession._chHP);
    sendPacket_Unicast(&createSession, &packet);
    packet.clear();

    // 기존 유저들에게 신규 유저 위치 보내기
    npfCreateOtherCharacter(&packet, createSession._dwSessionID, createSession._byDirection,
        createSession._shX, createSession._shY, createSession._chHP);
    sendPacket_Broadcast(&createSession, &packet);
    packet.clear();

    // 신규 유저에게 기존 유저 위치들 전송
    for (auto& session : g_sessionList)
    {
        if (session._dwSessionID != createSession._dwSessionID)
        {
            // 세션 많아지면 개선 필요
            npfCreateOtherCharacter(&packet, session._dwSessionID,
                session._byDirection, session._shX, session._shY, session._chHP);
            sendPacket_Unicast(&createSession, &packet);
            packet.clear();
        }
    }

    return true;
}

bool fdisconnect(st_SESSION* session)
{
    SerializationBuffer packet;
    npfDeleteCharacter(&packet, session->_dwSessionID);
    sendPacket_Broadcast(session, &packet);
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
            printf("recv fail\n");
            session->_chHP = 0;
        }
        return true;
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

        SerializationBuffer packet;
        peekRet = session->_recvQ.Peek(packet.getBufferPtr(), header->_bySize);
        if (peekRet != header->_bySize)
        {
            __debugbreak();
        }
        session->_recvQ.MoveFront(peekRet);
        packet.moveReadPos(peekRet);

        packetProc(session, header->_byType, &packet);
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
                printf("send fail\n");
                session->_chHP = 0;
            }
            return true;
        }

        session->_sendQ.MoveFront(sendRet);
    }
}

bool sendPacket_Unicast(st_SESSION* session, SerializationBuffer* packet)
{
    if (session->_sendQ.GetFreeSize() < packet->getDataSize())
    {
        return false;
    }

    int size = packet->getDataSize();
    int enqueueRet = session->_sendQ.Enqueue(packet->getBufferPtr(), size);
    if (enqueueRet != size)
    {
        session->_sendQ.Dequeue(packet->getBufferPtr(), enqueueRet);
        return false;
    }

    return true;
}

bool sendPacket_Broadcast(st_SESSION* session, SerializationBuffer* packet)
{
    for (auto& sessions : g_sessionList)
    {
        if (sessions._dwSessionID != session->_dwSessionID)
        {
            sendPacket_Unicast(&sessions, packet);
        }
    }

    return true;
}

bool packetProc(st_SESSION* session, unsigned char packetType, SerializationBuffer* packet)
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

bool netPacketProc_MoveStart(st_SESSION* session, SerializationBuffer* packet)
{
    unsigned char direction;
    short x;
    short y;

    *packet >> direction >> x >> y;

    if (abs(x - session->_shX) > dfERROR_RANGE || abs(y - session->_shY) > dfERROR_RANGE)
    {
        session->_chHP = 0;
        printf("Don't move location!!\n");
        return false;
    }

    session->_dwAction = direction;

    switch (direction)
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
    session->_shX = x;
    session->_shY = y;

    SerializationBuffer sendPacket;
    npfMoveStart(&sendPacket, session->_dwSessionID, direction, session->_shX, session->_shY);
    sendPacket_Broadcast(session, &sendPacket);

    return true;
}

bool netPacketProc_MoveStop(st_SESSION* session, SerializationBuffer* packet)
{
    unsigned char direction;
    short x;
    short y;

    *packet >> direction >> x >> y;

    if (abs(x - session->_shX) > dfERROR_RANGE || abs(y - session->_shY) > dfERROR_RANGE)
    {
        session->_chHP;
        printf("Don't move location!!\n");
        return false;
    }

    session->_dwAction = dfPACKET_CS_MOVE_STOP;

    switch (direction)
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
    session->_shX = x;
    session->_shY = y;

    SerializationBuffer sendPacket;
    npfMoveStop(&sendPacket, session->_dwSessionID, session->_byDirection,
        session->_shX, session->_shY);
    sendPacket_Broadcast(session, &sendPacket);

    return true;
}

bool netPacketProc_Attack1(st_SESSION* session, SerializationBuffer* packet)
{
    // 개선 필요
    // 공격 쿨타임
    unsigned char direction;
    short x;
    short y;

    *packet >> direction >> x >> y;

    if (direction != session->_byDirection || x != session->_shX || y != session->_shY)
    {
        session->_byDirection = direction;
        session->_shX = x;
        session->_shY = y;
    }

    SerializationBuffer sendPacket;
    npfAttack1(&sendPacket, session->_dwSessionID, session->_byDirection, session->_shX, session->_shY);
    sendPacket_Broadcast(session, &sendPacket);

    for (auto& sessions : g_sessionList)
    {
        if (sessions._dwSessionID != session->_dwSessionID)
        {
            if (direction == dfPACKET_MOVE_DIR_LL)
            {
                int minX = x - 80;
                int maxX = x;
                int minY = y - 10;
                int maxY = y + 10;
                if (sessions._shX >= minX && sessions._shX <= maxX &&
                    sessions._shY >= minY && sessions._shY <= maxY)
                {
                    SerializationBuffer sendPacket;
                    sessions._chHP -= 1;
                    npfDamage(&sendPacket, session->_dwSessionID, sessions._dwSessionID, sessions._chHP);
                    sendPacket_Unicast(session, &sendPacket);
                    sendPacket_Broadcast(session, &sendPacket);
                }
            }
            else if (direction == dfPACKET_MOVE_DIR_RR)
            {
                int maxX = x + 80;
                int minX = x;
                int minY = y - 10;
                int maxY = y + 10;
                if (sessions._shX >= minX && sessions._shX <= maxX &&
                    sessions._shY >= minY && sessions._shY <= maxY)
                {
                    SerializationBuffer sendPacket;
                    sessions._chHP -= 1;
                    npfDamage(&sendPacket, session->_dwSessionID, sessions._dwSessionID, sessions._chHP);
                    sendPacket_Unicast(session, &sendPacket);
                    sendPacket_Broadcast(session, &sendPacket);
                }
            }
        }
    }
    
    return true;
}

bool netPacketProc_Attack2(st_SESSION* session, SerializationBuffer* packet)
{
    // 개선 필요
    // 공격 쿨타임
    unsigned char direction;
    short x;
    short y;

    *packet >> direction >> x >> y;

    if (direction != session->_byDirection || x != session->_shX || y != session->_shY)
    {
        session->_byDirection = direction;
        session->_shX = x;
        session->_shY = y;
    }

    SerializationBuffer sendPacket;
    npfAttack2(&sendPacket, session->_dwSessionID, session->_byDirection, session->_shX, session->_shY);
    sendPacket_Broadcast(session, &sendPacket);

    for (auto& sessions : g_sessionList)
    {
        if (sessions._dwSessionID != session->_dwSessionID)
        {
            if (direction == dfPACKET_MOVE_DIR_LL)
            {
                int minX = x - 90;
                int maxX = x;
                int minY = y - 10;
                int maxY = y + 10;
                if (sessions._shX >= minX && sessions._shX <= maxX &&
                    sessions._shY >= minY && sessions._shY <= maxY)
                {
                    SerializationBuffer sendPacket;
                    sessions._chHP -= 1;
                    npfDamage(&sendPacket, session->_dwSessionID, sessions._dwSessionID, sessions._chHP);
                    sendPacket_Unicast(session, &sendPacket);
                    sendPacket_Broadcast(session, &sendPacket);
                }
            }
            else if (direction == dfPACKET_MOVE_DIR_RR)
            {
                int maxX = x + 90;
                int minX = x;
                int minY = y - 10;
                int maxY = y + 10;
                if (sessions._shX >= minX && sessions._shX <= maxX &&
                    sessions._shY >= minY && sessions._shY <= maxY)
                {
                    SerializationBuffer sendPacket;
                    sessions._chHP -= 1;
                    npfDamage(&sendPacket, session->_dwSessionID, sessions._dwSessionID, sessions._chHP);
                    sendPacket_Unicast(session, &sendPacket);
                    sendPacket_Broadcast(session, &sendPacket);
                }
            }
        }
    }

    return true;
}

bool netPacketProc_Attack3(st_SESSION* session, SerializationBuffer* packet)
{
    // 개선 필요
    // 공격 쿨타임
    unsigned char direction;
    short x;
    short y;

    *packet >> direction >> x >> y;

    if (direction != session->_byDirection || x != session->_shX || y != session->_shY)
    {
        session->_byDirection = direction;
        session->_shX = x;
        session->_shY = y;
    }

    SerializationBuffer sendPacket;
    npfAttack3(&sendPacket, session->_dwSessionID, session->_byDirection, session->_shX, session->_shY);
    sendPacket_Broadcast(session, &sendPacket);

    for (auto& sessions : g_sessionList)
    {
        if (sessions._dwSessionID != session->_dwSessionID)
        {
            if (direction == dfPACKET_MOVE_DIR_LL)
            {
                int minX = x - 100;
                int maxX = x;
                int minY = y - 20;
                int maxY = y + 20;
                if (sessions._shX >= minX && sessions._shX <= maxX &&
                    sessions._shY >= minY && sessions._shY <= maxY)
                {
                    SerializationBuffer sendPacket;
                    sessions._chHP -= 1;
                    npfDamage(&sendPacket, session->_dwSessionID, sessions._dwSessionID, sessions._chHP);
                    sendPacket_Unicast(session, &sendPacket);
                    sendPacket_Broadcast(session, &sendPacket);
                }
            }
            else if (direction == dfPACKET_MOVE_DIR_RR)
            {
                int maxX = x + 100;
                int minX = x;
                int minY = y - 20;
                int maxY = y + 20;
                if (sessions._shX >= minX && sessions._shX <= maxX &&
                    sessions._shY >= minY && sessions._shY <= maxY)
                {
                    SerializationBuffer sendPacket;
                    sessions._chHP -= 1;
                    npfDamage(&sendPacket, session->_dwSessionID, sessions._dwSessionID, sessions._chHP);
                    sendPacket_Unicast(session, &sendPacket);
                    sendPacket_Broadcast(session, &sendPacket);
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
    // 시간 단위
    /*unsigned long now = timeGetTime();
    float deltaSec = (now - g_updateTime) / 1000.0f;
    g_updateTime = now;

    int moveAmountX = (int)floor(c_move_speed_X * deltaSec);
    int moveAmountY = (int)floor(c_move_speed_Y * deltaSec);
    int moveAmountDiagX = (int)floor(c_move_speed_DIAG_X * deltaSec);
    int moveAmountDiagY = (int)floor(c_move_speed_DIAG_Y * deltaSec);
    
    for (auto iter = g_sessionList.begin(); iter != g_sessionList.end();)
    {
        if (iter->_chHP <= 0)
        {
            fdisconnect(&(*iter));
            iter = g_sessionList.erase(iter);
        }
        else
        {
            switch (iter->_dwAction)
            {
            case dfPACKET_MOVE_DIR_LL:
                if (iter->_shX - moveAmountX <= dfRANGE_MOVE_LEFT)
                {
                    break;
                }
                iter->_shX -= moveAmountX;
                break;
            case dfPACKET_MOVE_DIR_LU:
                if (iter->_shX - moveAmountDiagX <= dfRANGE_MOVE_LEFT
                    || iter->_shY - moveAmountDiagY <= dfRANGE_MOVE_TOP)
                {
                    break;
                }
                iter->_shX -= moveAmountDiagX;
                iter->_shY -= moveAmountDiagY;
                break;
            case dfPACKET_MOVE_DIR_UU:
                if (iter->_shY - moveAmountY <= dfRANGE_MOVE_TOP)
                {
                    break;
                }
                iter->_shY -= moveAmountY;
                break;
            case dfPACKET_MOVE_DIR_RU:
                if (iter->_shX + moveAmountDiagX >= dfRANGE_MOVE_RIGHT
                    || iter->_shY - moveAmountDiagY <= dfRANGE_MOVE_TOP)
                {
                    break;
                }
                iter->_shX += moveAmountDiagX;
                iter->_shY -= moveAmountDiagY;
                break;
            case dfPACKET_MOVE_DIR_RR:
                if (iter->_shX + moveAmountX >= dfRANGE_MOVE_RIGHT)
                {
                    break;
                }
                iter->_shX += moveAmountX;
                break;
            case dfPACKET_MOVE_DIR_RD:
                if (iter->_shX + moveAmountDiagX >= dfRANGE_MOVE_RIGHT
                    || iter->_shY + moveAmountDiagY >= dfRANGE_MOVE_BOTTOM)
                {
                    break;
                }
                iter->_shX += moveAmountDiagX;
                iter->_shY += moveAmountDiagY;
                break;
            case dfPACKET_MOVE_DIR_DD:
                if (iter->_shY + moveAmountY >= dfRANGE_MOVE_BOTTOM)
                {
                    break;
                }
                iter->_shY += moveAmountY;
                break;
            case dfPACKET_MOVE_DIR_LD:
                if (iter->_shX - moveAmountDiagX <= dfRANGE_MOVE_LEFT
                    || iter->_shY + moveAmountDiagY >= dfRANGE_MOVE_BOTTOM)
                {
                    break;
                }
                iter->_shX -= moveAmountDiagX;
                iter->_shY += moveAmountDiagY;
                break;
            }

            ++iter;
        }
    }*/


    // 프레임 단위
    for (auto iter = g_sessionList.begin(); iter != g_sessionList.end();)
    {
        if (iter->_chHP <= 0)
        {
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
                break;
            case dfPACKET_MOVE_DIR_LU:
                if (iter->_shX - 3 <= dfRANGE_MOVE_LEFT 
                    || iter->_shY - 2 <= dfRANGE_MOVE_TOP)
                {
                    break;
                }
                iter->_shX -= 3;
                iter->_shY -= 2;
                break;
            case dfPACKET_MOVE_DIR_UU:
                if (iter->_shY - 2 <= dfRANGE_MOVE_TOP)
                {
                    break;
                }
                iter->_shY -= 2;
                break;
            case dfPACKET_MOVE_DIR_RU:
                if (iter->_shX + 3 >= dfRANGE_MOVE_RIGHT
                    || iter->_shY - 2 <= dfRANGE_MOVE_TOP)
                {
                    break;
                }
                iter->_shX += 3;
                iter->_shY -= 2;
                break;
            case dfPACKET_MOVE_DIR_RR:
                if (iter->_shX + 3 >= dfRANGE_MOVE_RIGHT)
                {
                    break;
                }
                iter->_shX += 3;
                break;
            case dfPACKET_MOVE_DIR_RD:
                if (iter->_shX + 3 >= dfRANGE_MOVE_RIGHT
                    || iter->_shY + 2 >= dfRANGE_MOVE_BOTTOM)
                {
                    break;
                }
                iter->_shX += 3;
                iter->_shY += 2;
                break;
            case dfPACKET_MOVE_DIR_DD:
                if (iter->_shY + 2 >= dfRANGE_MOVE_BOTTOM)
                {
                    break;
                }
                iter->_shY += 2;
                break;
            case dfPACKET_MOVE_DIR_LD:
                if (iter->_shX - 3 <= dfRANGE_MOVE_LEFT
                    || iter->_shY + 2 >= dfRANGE_MOVE_BOTTOM)
                {
                    break;
                }
                iter->_shX -= 3;
                iter->_shY += 2;
                break;
            }

            ++iter;
        }
    }

    return true;
}

bool npfCreateCharacter(SerializationBuffer* packet, unsigned int id, unsigned char direction, short x, short y, char hp)
{
    st_HEADER header;
    header._byCode = ﻿dfNETWORK_PACKET_CODE;
    header._bySize = sizeof(st_SC_CREATE_MY_CHARACTER);
    header._byType = dfPACKET_SC_CREATE_MY_CHARACTER;

    packet->putData((char*)&header, sizeof(header));

    *packet << id;
    *packet << direction;
    *packet << x;
    *packet << y;
    *packet << hp;

    return true;
}

bool npfCreateOtherCharacter(SerializationBuffer* packet, unsigned int id, unsigned char direction, short x, short y, char hp)
{
    st_HEADER header;
    header._byCode = ﻿dfNETWORK_PACKET_CODE;
    header._bySize = sizeof(st_SC_CREATE_OTHER_CHARACTER);
    header._byType = dfPACKET_SC_CREATE_OTHER_CHARACTER;

    packet->putData((char*)&header, sizeof(header));

    *packet << id;
    *packet << direction;
    *packet << x;
    *packet << y;
    *packet << hp;

    return true;
}

bool npfDeleteCharacter(SerializationBuffer* packet, unsigned int id)
{
    st_HEADER header;
    header._byCode = ﻿dfNETWORK_PACKET_CODE;
    header._bySize = sizeof(st_SC_DELETE_CHARACTER);
    header._byType = dfPACKET_SC_DELETE_CHARACTER;

    packet->putData((char*)&header, sizeof(header));

    *packet << id;

    return true;
}

bool npfMoveStart(SerializationBuffer* packet, unsigned int id, unsigned char direction, short x, short y)
{
    st_HEADER header;
    header._byCode = ﻿dfNETWORK_PACKET_CODE;
    header._bySize = sizeof(st_SC_MOVE_START);
    header._byType = dfPACKET_SC_MOVE_START;

    packet->putData((char*)&header, sizeof(header));

    *packet << id;
    *packet << direction;
    *packet << x;
    *packet << y;

    return true;
}

bool npfMoveStop(SerializationBuffer* packet, unsigned int id, unsigned char direction, short x, short y)
{
    st_HEADER header;
    header._byCode = ﻿dfNETWORK_PACKET_CODE;
    header._bySize = sizeof(st_SC_MOVE_STOP);
    header._byType = dfPACKET_SC_MOVE_STOP;

    packet->putData((char*)&header, sizeof(header));

    *packet << id;
    *packet << direction;
    *packet << x;
    *packet << y;

    return true;
}

bool npfAttack1(SerializationBuffer* packet, unsigned int id, unsigned char direction, short x, short y)
{
    st_HEADER header;
    header._byCode = ﻿dfNETWORK_PACKET_CODE;
    header._bySize = sizeof(st_SC_ATTACK1);
    header._byType = dfPACKET_SC_ATTACK1;

    packet->putData((char*)&header, sizeof(header));

    *packet << id;
    *packet << direction;
    *packet << x;
    *packet << y;

    return true;
}

bool npfAttack2(SerializationBuffer* packet, unsigned int id, unsigned char direction, short x, short y)
{
    st_HEADER header;
    header._byCode = ﻿dfNETWORK_PACKET_CODE;
    header._bySize = sizeof(st_SC_ATTACK2);
    header._byType = dfPACKET_SC_ATTACK2;

    packet->putData((char*)&header, sizeof(header));

    *packet << id;
    *packet << direction;
    *packet << x;
    *packet << y;

    return true;
}

bool npfAttack3(SerializationBuffer* packet, unsigned int id, unsigned char direction, short x, short y)
{
    st_HEADER header;
    header._byCode = ﻿dfNETWORK_PACKET_CODE;
    header._bySize = sizeof(st_SC_ATTACK3);
    header._byType = dfPACKET_SC_ATTACK3;

    packet->putData((char*)&header, sizeof(header));

    *packet << id;
    *packet << direction;
    *packet << x;
    *packet << y;

    return true;
}

bool npfDamage(SerializationBuffer* packet, unsigned int attackID, unsigned int damageID, char damageHP)
{
    st_HEADER header;
    header._byCode = ﻿dfNETWORK_PACKET_CODE;
    header._bySize = sizeof(st_SC_DAMAGE);
    header._byType = dfPACKET_SC_DAMAGE;

    packet->putData((char*)&header, sizeof(header));

    *packet << attackID;
    *packet << damageID;
    *packet << damageHP;

    return true;
}