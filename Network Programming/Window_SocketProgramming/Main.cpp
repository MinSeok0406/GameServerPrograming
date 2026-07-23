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

#define SERVERPORT  47000
#define BUFSIZE     512
#define CLIENT      100

struct SESSION
{
    SOCKET _sock;
    wchar_t _ip[INET_ADDRSTRLEN];
    short _port;
    bool _live;
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
IDALLOC g_idalloc[CLIENT];
CREATESTAR g_createstar[CLIENT];
STARMOVE g_starmove[CLIENT];

fd_set rset;
SOCKET listen_sock;
static int s_idalloc = 0;
static int s_create = 0;
static int s_move = 0;
char buf[BUFSIZE];

void sendUnicast(SESSION* s, char* bbuf);      // Session에는 보낼 세션만을 넣어야함
void sendBroadcast(SESSION* s, char* bbuf);    // Session에는 보내지 않을 세션을 넣어야함
int network();
int render();

int wmain(int argc, WCHAR* argv[])
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

    timeBeginPeriod(1);
    srand(unsigned int(time(nullptr)));
    cs_Initial();

    memset(g_idalloc, -1, sizeof(g_idalloc));
    memset(g_createstar, -1, sizeof(g_createstar));
    memset(g_starmove, -1, sizeof(g_starmove));

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        return 1;
    }

    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET)
    {
        //wprintf(L"%d\n", WSAGetLastError());
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
        //wprintf(L"%d\n", WSAGetLastError());
        return 1;
    }

    u_long on = 1;
    int nbioret = ioctlsocket(listen_sock, FIONBIO, &on);
    if (nbioret == SOCKET_ERROR)
    {
        //wprintf(L"%d\n", WSAGetLastError());
        return 1;
    }

    int listenret = listen(listen_sock, SOMAXCONN);
    if (listenret == SOCKET_ERROR)
    {
        //wprintf(L"%d\n", WSAGetLastError());
        return 1;
    }

    while (true)
    {
        network();

        render();
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
        //wprintf(L"%d\n", WSAGetLastError());
        return;
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
        //wprintf(L"%d\n", WSAGetLastError());
        return 0;
    }

    if (FD_ISSET(listen_sock, &rset))
    {
        SESSION session = { 0, };
        SOCKET client_sock;
        SOCKADDR_IN clientaddr;
        int addrlen = sizeof(clientaddr);

        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET)
        {
            //wprintf(L"%d\n", WSAGetLastError());
            return 0;
        }

        session._sock = client_sock;
        wchar_t addr[INET_ADDRSTRLEN];
        InetNtop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
        wcscpy(session._ip, addr);
        session._port = ntohs(clientaddr.sin_port);
        session._live = true;
        session._id = s_idalloc;
        g_idalloc[s_idalloc]._type = 0;
        g_idalloc[s_idalloc]._id = s_idalloc;
        g_idalloc[s_idalloc]._g = 0;
        g_idalloc[s_idalloc]._g2 = 0;

        sendUnicast(&session, (char*)&g_idalloc[s_idalloc]);
        s_idalloc++;

        int x = rand() % dfSCREEN_WIDTH;
        int y = rand() % dfSCREEN_HEIGHT;
        session._x = x;
        session._y = y;
        g_createstar[s_create]._type = 1;
        g_createstar[s_create]._id = session._id;
        g_createstar[s_create]._x = x;
        g_createstar[s_create]._y = y;

        // 신규 유저에게 별 생성 위치 송신
        sendUnicast(&session, (char*)&g_createstar[s_create]);

        // 신규 유저에게 다른 별들 위치 송신
        for (auto i = 0; i < s_move; ++i)
        {
            if (g_starmove[i]._id != session._id)
            {
                sendUnicast(&session, (char*)&g_starmove[i]);
            }
        }

        // 신규 유저 등록
        g_playerList.push_back(session);

        // 다른 유저에게 신규 유저 위치 알려주기
        sendBroadcast(&session, (char*)&g_createstar[s_create]);
        s_create++;

        g_starmove[s_move]._type = 3;
        g_starmove[s_move]._id = session._id;
        g_starmove[s_move]._x = session._x;
        g_starmove[s_move]._y = session._y;
        s_move++;
    }

    for (auto& session : g_playerList)
    {
        if (FD_ISSET(session._sock, &rset))
        {
            int recvret = recv(session._sock, buf, sizeof(buf), 0);
            if (recvret == SOCKET_ERROR)
            {
                //wprintf(L"%d\n", WSAGetLastError());
                session._live = false;
                continue;
            }
            else if (recvret == 0)
            {
                session._live = false;
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
                    for (auto i = 0; i < s_move; ++i)
                    {
                        if (g_starmove[i]._id == id)
                        {
                            g_starmove[i]._x = x;
                            g_starmove[i]._y = y;
                            session._x = x;
                            session._y = y;
                            sendBroadcast(&session, (char*)&g_starmove[i]);
                            break;
                        }
                    }
                    break;
                }

                sum += 16;
            }
        }
    }

    for (auto iter = g_playerList.begin(); iter != g_playerList.end();)
    {
        if (iter->_live == false)
        {
            DELETESTAR deleteStar;
            deleteStar._type = 2;
            deleteStar._id = iter->_id;
            deleteStar._g = 0;
            deleteStar._g2 = 0;

            sendBroadcast(&(*iter), (char*)&deleteStar);

            iter = g_playerList.erase(iter);
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
    for (auto& session : g_playerList)
    {
        Sprite_Draw(session._x, session._y, L'*');
    }
    Buffer_Flip();

    return 1;
}