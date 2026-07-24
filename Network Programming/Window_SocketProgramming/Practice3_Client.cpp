/*#include <iostream>
#include <time.h>
#include <fcntl.h>
#include <io.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <vector>
#include "Buffer.h"
#include "Console.h"
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

char SERVERIP[INET_ADDRSTRLEN];
#define SERVERPORT  47000
#define BUFSIZE     160
#define CLIENT      1000

struct Player
{
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

Player g_player(-1, -1, -1);
IDALLOC g_idalloc(0, 0, 0, 0);
CREATESTAR g_createstar[CLIENT];
DELETESTAR g_deletestar[CLIENT];
STARMOVE g_starmove[CLIENT];

fd_set rset;
timeval t;
SOCKET server_sock;
int retval;
int recvByte;
int useTime;
DWORD tm;
static int s_create = 0;
static int s_delete = 0;
static int s_move = 0;
char buf[BUFSIZE];

int inputMove();
int networkLogic();
int Render();

int wmain(int argc, WCHAR* argv[])
{
    timeBeginPeriod(1);
    srand(unsigned int(time(nullptr)));
    cs_Initial();

    memset(g_createstar, -1, sizeof(g_createstar));
    memset(g_deletestar, -1, sizeof(g_deletestar));
    memset(g_starmove, -1, sizeof(g_starmove));

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 1;
    }

    printf("접속할 IP 주소를 입력해주세요 : ");
    fgets(SERVERIP, INET_ADDRSTRLEN, stdin);

    int len = (int)strlen(SERVERIP);
    if (SERVERIP[len - 1] == '\n')
    {
        SERVERIP[len - 1] = '\0';
    }

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == INVALID_SOCKET)
    {
        //wprintf(L"%d\n", WSAGetLastError());
        return 1;
    }

    SOCKADDR_IN serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERVERPORT);
    inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
    //InetPton(AF_INET, SERVERIP, &serveraddr.sin_addr);

    retval = connect(server_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR)
    {
        //wprintf(L"%d\n", WSAGetLastError());
        return 1;
    }

    u_long on = 1;
    retval = ioctlsocket(server_sock, FIONBIO, &on);
    if (retval == SOCKET_ERROR)
    {
        //wprintf(L"%d\n", WSAGetLastError());
        return 1;
    }

    tm = timeGetTime();

    while (true)
    {
        if (!inputMove())
        {
            break;
        }

        if (!networkLogic())
        {
            break;
        }

        Render();

        useTime = (int)(timeGetTime() - tm);
        if (useTime < 10)
        {
            Sleep(10 - useTime);
        }
        tm += 10;
    }

    closesocket(server_sock);
    WSACleanup();

    return 0;
}

int inputMove()
{
    if (g_player._id == -1)
    {
        return 1;
    }

    bool flag = false;
    if (GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_UP))
    {
        if (g_player._x - 1 == -1 || g_player._y - 1 == -1)
        {
            return 1;
        }

        g_player._x -= 1;
        g_player._y -= 1;
        flag = true;
    }
    else if (GetAsyncKeyState(VK_UP) && GetAsyncKeyState(VK_RIGHT))
    {
        if (g_player._x + 2 == dfSCREEN_WIDTH || g_player._y - 1 == -1)
        {
            return 1;
        }

        g_player._x += 1;
        g_player._y -= 1;
        flag = true;
    }
    else if (GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_DOWN))
    {
        if (g_player._x + 2 == dfSCREEN_WIDTH || g_player._y + 1 == dfSCREEN_HEIGHT)
        {
            return 1;
        }

        g_player._x += 1;
        g_player._y += 1;
        flag = true;
    }
    else if (GetAsyncKeyState(VK_DOWN) && GetAsyncKeyState(VK_LEFT))
    {
        if (g_player._x - 1 == -1 || g_player._y + 1 == dfSCREEN_HEIGHT)
        {
            return 1;
        }

        g_player._x -= 1;
        g_player._y += 1;
        flag = true;
    }
    else if (GetAsyncKeyState(VK_LEFT))
    {
        if (g_player._x - 1 == -1)
        {
            return 1;
        }

        g_player._x -= 1;
        flag = true;
    }
    else if (GetAsyncKeyState(VK_UP))
    {
        if (g_player._y - 1 == -1)
        {
            return 1;
        }

        g_player._y -= 1;
        flag = true;
    }
    else if (GetAsyncKeyState(VK_RIGHT))
    {
        if (g_player._x + 2 == dfSCREEN_WIDTH)
        {
            return 1;
        }

        g_player._x += 1;
        flag = true;
    }
    else if (GetAsyncKeyState(VK_DOWN))
    {
        if (g_player._y + 1 == dfSCREEN_HEIGHT)
        {
            return 1;
        }

        g_player._y += 1;
        flag = true;
    }

    if (flag)
    {
        // send
        for (auto i = 0; i < s_move; ++i)
        {
            if (g_starmove[i]._id == g_player._id)
            {
                g_starmove[i]._x = g_player._x;
                g_starmove[i]._y = g_player._y;

                retval = send(server_sock, (char*)&g_starmove[i], sizeof(STARMOVE), 0);
                if (retval == SOCKET_ERROR)
                {
                    //wprintf(L"%d\n", WSAGetLastError());
                    return 0;
                }

                break;
            }
        }
    }

    return 1;
}

int networkLogic()
{
    FD_ZERO(&rset);
    FD_SET(server_sock, &rset);
    t.tv_sec = 0;
    t.tv_usec = 1000;

    retval = select(0, &rset, NULL, NULL, &t);
    if (retval == SOCKET_ERROR)
    {
        //wprintf(L"%d\n", WSAGetLastError());
        return 0;
    }

    if (FD_ISSET(server_sock, &rset))
    {
        retval = recv(server_sock, buf, sizeof(buf), 0);
        if (retval == SOCKET_ERROR)
        {
            //wprintf(L"%d\n", WSAGetLastError());
            return 0;
        }
        else if (retval == 0)
        {
            return 0;
        }

        recvByte = retval;
        int sum = 0;

        while (recvByte - sum >= 16)
        {
            int* bbuf = (int*)(buf + sum);
            int type = bbuf[0];
            int id = bbuf[1];
            int x = bbuf[2];
            int y = bbuf[3];

            bool isCreate = true;
            bool isMove = true;
            switch (type)
            {
            case 0:
                g_player._id = id;
                g_idalloc._id = id;
                break;
            case 1:
                for (auto i = 0; i < s_create; ++i)
                {
                    if (g_createstar[i]._id == id)
                    {
                        isCreate = false;
                        break;
                    }
                }

                for (auto i = 0; i < s_move; ++i)
                {
                    if (g_starmove[i]._id == id)
                    {
                        isMove = false;
                        break;
                    }
                }

                if (isCreate)
                {
                    g_createstar[s_create++] = { 1, id, x, y };
                }

                if (isMove)
                {
                    g_starmove[s_move++] = { 3, id, x, y };
                }

                if (id == g_player._id)
                {
                    g_player._x = x;
                    g_player._y = y;
                }
                break;
            case 2:
                g_deletestar[s_delete++] = { 2, id, 0, 0 };
                break;
            case 3:
                for (auto i = 0; i < s_move; ++i)
                {
                    if (g_starmove[i]._id == id)
                    {
                        g_starmove[i]._x = x;
                        g_starmove[i]._y = y;
                        isMove = false;
                        break;
                    }
                }

                if (isMove)
                {
                    g_starmove[s_move++] = { 3, id, x, y };
                }

                break;
            }

            sum += 16;
        }
    }

    return 1;
}

int Render()
{
    if (g_player._x == -1)
    {
        return 1;
    }

    Buffer_Clear();

    for (auto i = 0; i < s_move; ++i)
    {
        if (g_starmove[i]._id == -1)
        {
            break;
        }

        bool isDelete = false;
        for (auto j = 0; j < s_delete; ++j)
        {
            if (g_starmove[i]._id == g_deletestar[j]._id)
            {
                isDelete = true;
                break;
            }
        }

        if (!isDelete)
        {
            Sprite_Draw(g_starmove[i]._x, g_starmove[i]._y, '*');
        }
    }

    Buffer_Flip();

    return 1;
}*/