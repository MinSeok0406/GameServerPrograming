#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <time.h>
#include <fcntl.h>
#include <io.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <list>
#include "Console.h"
#include "Buffer.h"
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

#define SERVERPORT  3000
#define BUFSIZE     160
#define CLIENT      100

DWORD tick = timeGetTime();
void FPS()
{
    static int cnt;

    if (timeGetTime() - tick > 1000)
    {
        printf("FPS : %d\n", cnt);
        cnt = 0;
        tick += 1000;
    }

    cnt++;
}

struct SESSION
{
    SOCKET _sock;
    wchar_t _ip[INET_ADDRSTRLEN];
    u_short _port;
    int _id;
    int _x;
    int _y;
};

struct IDALLOC
{
    int _type;
    int _id;
    int _g;
    int _g2;
};

struct CREATESTAR
{
    int _type;
    int _id;
    int _x;
    int _y;
};

struct DELETESTAR
{
    int _type;
    int _id;
    int _g;
    int _g2;
};

struct STARMOVE
{
    int _type;
    int _id;
    int _x;
    int _y;
};

list<SESSION> g_playerList;
list<IDALLOC> g_idalloc;
list<CREATESTAR> g_createstar;
list<DELETESTAR> g_deletestar;
list<STARMOVE> g_starmove;

fd_set rset;
SOCKET listen_sock;
u_int useTime;
DWORD tm;
char buf[BUFSIZE];
static int s_id = 0;

void sendUnicast(SESSION* s, char* bbuf);      // Session에는 보낼 세션만을 넣어야함
void sendBroadcast(SESSION* s, char* bbuf);    // Session에는 보내지 않을 세션을 넣어야함
int network();
int render();

int main(int argc, CHAR* argv[])
{
    timeBeginPeriod(1);
    srand(unsigned int(time(nullptr)));
    cs_Initial();

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        return 1;
    }

    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET)
    {
        printf("%d\n", WSAGetLastError());
        return 1;
    }

    SOCKADDR_IN serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    int bindret = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (bindret == SOCKET_ERROR)
    {
        printf("%d\n", WSAGetLastError());
        return 1;
    }

    u_long on = 1;
    int nbioret = ioctlsocket(listen_sock, FIONBIO, &on);
    if (nbioret == SOCKET_ERROR)
    {
        printf("%d\n", WSAGetLastError());
        return 1;
    }

    int listenret = listen(listen_sock, SOMAXCONN);
    if (listenret == SOCKET_ERROR)
    {
        printf("%d\n", WSAGetLastError());
        return 1;
    }

    tm = timeGetTime();

    while (true)
    {
        network();

        render();

        useTime = timeGetTime() - tm;
        if (useTime < 10)
        {
            Sleep(10 - useTime);
        }
        else
        {
            useTime = timeGetTime();
        }
        tm += 10;
    }

    closesocket(listen_sock);
    WSACleanup();

    return 0;
}

void sendUnicast(SESSION* s, char* bbuf)
{
    int sendret = send(s->_sock, bbuf, 16, 0);
    if (sendret == SOCKET_ERROR)
    {
        if (WSAGetLastError() == WSAEWOULDBLOCK)
        {
            return;
        }
        else
        {
            printf("%d\n", WSAGetLastError());
        }
    }
}

void sendBroadcast(SESSION* s, char* bbuf)
{
    for (auto& session : g_playerList)
    {
        if (session._id != s->_id)
        {
            sendUnicast(&session, bbuf);
        }
    }
}

int network()
{
    FD_ZERO(&rset);
    FD_SET(listen_sock, &rset);

    for (auto& session : g_playerList)
    {
        FD_SET(session._sock, &rset);
    }

    int selectret = select(0, &rset, NULL, NULL, NULL);
    if (selectret == SOCKET_ERROR)
    {
        if (WSAGetLastError() == WSAEWOULDBLOCK)
        {
            return 0;
        }
        else
        {
            printf("%d\n", WSAGetLastError());
            return 0;
        }
    }

    if (FD_ISSET(listen_sock, &rset))
    {
        SESSION session = { 0, };
        SOCKET client_sock;
        SOCKADDR_IN clientaddr;
        int addrlen = sizeof(clientaddr);

        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAEWOULDBLOCK)
            {
                return 0;
            }
            else
            {
                printf("%d\n", WSAGetLastError());
                return 0;
            }
        }

        session._sock = client_sock;
        wchar_t addr[INET_ADDRSTRLEN];
        InetNtop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
        wcscpy(session._ip, addr);
        session._port = ntohs(clientaddr.sin_port);
        session._id = s_id;
        IDALLOC idalloc { 0, s_id, 0, 0 };
        g_idalloc.push_back(idalloc);

        sendUnicast(&session, (char*)&idalloc);
        s_id++;

        int x = rand() % (dfSCREEN_WIDTH - 1);
        int y = rand() % dfSCREEN_HEIGHT;
        session._x = x;
        session._y = y;
        CREATESTAR createstar { 1, session._id, session._x, session._y };
        g_createstar.push_back(createstar);

        // 신규 유저에게 별 생성 위치 송신
        sendUnicast(&session, (char*)&createstar);

        // 신규 유저에게 다른 별들 위치 송신
        for (auto& starmove : g_starmove)
        {
            if (starmove._id != session._id)
            {
                sendUnicast(&session, (char*)&starmove);
            }
        }

        // 신규 유저 등록
        g_playerList.push_back(session);

        // 다른 유저에게 신규 유저 위치 알려주기
        sendBroadcast(&session, (char*)&createstar);

        STARMOVE starmove { 3, session._id, session._x, session._y };
        g_starmove.push_back(starmove);
    }

    for (auto& session : g_playerList)
    {
        if (FD_ISSET(session._sock, &rset))
        {
            int recvret = recv(session._sock, buf, sizeof(buf), 0);
            if (recvret == SOCKET_ERROR)
            {
                if (WSAGetLastError() == WSAEWOULDBLOCK)
                {
                    continue;
                }
                else
                {
                    printf("%d\n", WSAGetLastError());
                    DELETESTAR deletestar { 2, session._id, 0, 0 };
                    sendBroadcast(&session, (char*)&deletestar);
                    g_deletestar.push_back(deletestar);
                }

                continue;
            }
            else if (recvret == 0)
            {
                printf("%d\n", WSAGetLastError());
                DELETESTAR deletestar { 2, session._id, 0, 0 };
                sendBroadcast(&session, (char*)&deletestar);
                g_deletestar.push_back(deletestar);
                continue;
            }

            int recvByte = recvret;
            int sum = 0;

            while (recvByte - sum >= 16)
            {
                int* bbuf = (int*)(buf + sum);
                int type = bbuf[0];
                int id = bbuf[1];
                int x = bbuf[2];
                int y = bbuf[3];

                switch (type)
                {
                case 3:
                    for (auto& starmove : g_starmove)
                    {
                        if (starmove._id == id)
                        {
                            starmove._x = x;
                            starmove._y = y;
                            session._x = x;
                            session._y = y;
                            sendBroadcast(&session, (char*)&starmove);
                            break;
                        }
                    }
                    break;
                }

                sum += 16;
            }
        }
    }

    for (auto iter = g_deletestar.begin(); iter != g_deletestar.end();)
    {
        bool deletePlayer = false;
        // 플레이어 삭제
        for (auto msIter = g_playerList.begin(); msIter != g_playerList.end();)
        {
            if (msIter->_id == iter->_id)
            {
                msIter = g_playerList.erase(msIter);
                deletePlayer = true;
                break;
            }
            else
            {
                ++msIter;
            }
        }

        // 여러 리스트에서 이 아이디를 가진 오브젝트 삭제
        for (auto msIter = g_idalloc.begin(); msIter != g_idalloc.end();)
        {
            if (msIter->_id == iter->_id)
            {
                msIter = g_idalloc.erase(msIter);
                deletePlayer = true;
                break;
            }
            else
            {
                ++msIter;
            }
        }

        for (auto msIter = g_createstar.begin(); msIter != g_createstar.end();)
        {
            if (msIter->_id == iter->_id)
            {
                msIter = g_createstar.erase(msIter);
                deletePlayer = true;
                break;
            }
            else
            {
                ++msIter;
            }
        }

        for (auto msIter = g_starmove.begin(); msIter != g_starmove.end();)
        {
            if (msIter->_id == iter->_id)
            {
                msIter = g_starmove.erase(msIter);
                deletePlayer = true;
                break;
            }
            else
            {
                ++msIter;
            }
        }


        if (deletePlayer)
        {
            iter = g_deletestar.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    return 1;
}

int render()
{
    Buffer_Clear();
    //FPS();
    for (auto& session : g_playerList)
    {
        Sprite_Draw(session._x, session._y, '*');
    }
    Buffer_Flip();

    return 1;
}