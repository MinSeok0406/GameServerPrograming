/*#include <iostream>
#include <time.h>
#include <fcntl.h>
#include <io.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <list>
#include "Buffer.h"
#include "Console.h"
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

wchar_t SERVERIP[INET_ADDRSTRLEN];
#define SERVERPORT  3000
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
list<IDALLOC> g_idalloc;
list<CREATESTAR> g_createstar;
list<DELETESTAR> g_deletestar;
list<STARMOVE> g_starmove;

fd_set rset;
timeval t;
SOCKET server_sock;
int recvByte;
int useTime;
DWORD tm;
char buf[BUFSIZE];

int inputMove();
int networkLogic();
int Render();

int wmain(int argc, WCHAR* argv[])
{
    timeBeginPeriod(1);
    srand(unsigned int(time(nullptr)));
    cs_Initial();

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 1;
    }

    printf("접속할 IP 주소를 입력해주세요 : ");
    fgetws(SERVERIP, INET_ADDRSTRLEN, stdin);

    int len = (int)wcslen(SERVERIP);
    if (SERVERIP[len - 1] == '\n')
    {
        SERVERIP[len - 1] = '\0';
    }

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == INVALID_SOCKET)
    {
        wprintf(L"%d\n", WSAGetLastError());
        return 1;
    }

    SOCKADDR_IN serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERVERPORT);
    InetPton(AF_INET, SERVERIP, &serveraddr.sin_addr);

    int connectRet = connect(server_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (connectRet == SOCKET_ERROR)
    {
        wprintf(L"%d\n", WSAGetLastError());
        return 1;
    }

    u_long on = 1;
    int nonblkRet = ioctlsocket(server_sock, FIONBIO, &on);
    if (nonblkRet == SOCKET_ERROR)
    {
        wprintf(L"%d\n", WSAGetLastError());
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
        if (g_player._x + 1 == dfSCREEN_WIDTH - 1 || g_player._y - 1 == -1)
        {
            return 1;
        }

        g_player._x += 1;
        g_player._y -= 1;
        flag = true;
    }
    else if (GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_DOWN))
    {
        if (g_player._x + 1 == dfSCREEN_WIDTH - 1 || g_player._y + 1 == dfSCREEN_HEIGHT)
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
        if (g_player._x + 1 == dfSCREEN_WIDTH - 1)
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
        for (auto& starmove : g_starmove)
        {
            if (starmove._id == g_player._id)
            {
                starmove._x = g_player._x;
                starmove._y = g_player._y;

                int sendRet = send(server_sock, (char*)&starmove, sizeof(STARMOVE), 0);
                if (sendRet == SOCKET_ERROR)
                {
                    wprintf(L"%d\n", WSAGetLastError());
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

    int selectRet = select(0, &rset, NULL, NULL, &t);
    if (selectRet == SOCKET_ERROR)
    {
        wprintf(L"%d\n", WSAGetLastError());
        return 0;
    }

    if (FD_ISSET(server_sock, &rset))
    {
        int recvRet = recv(server_sock, buf, sizeof(buf), 0);
        if (recvRet == SOCKET_ERROR)
        {
            wprintf(L"%d\n", WSAGetLastError());
            return 0;
        }
        else if (recvRet == 0)
        {
            return 0;
        }

        recvByte = recvRet;
        int sum = 0;

        while (recvByte - sum >= 16)
        {
            int* bbuf = (int*)(buf + sum);
            int type = bbuf[0];
            int id = bbuf[1];
            int x = bbuf[2];
            int y = bbuf[3];

            bool moveAdd = true;
            switch (type)
            {
            case 0:
                g_player._id = id;
                g_idalloc.push_back({ 0, id, 0, 0 });
                break;
            case 1:
                // 내 캐릭터 + 다른 유저 위치 받아오기
                g_createstar.push_back({ 1, id, x, y });
                g_starmove.push_back({ 3, id, x, y });
                if (id == g_player._id)
                {
                    g_player._x = x;
                    g_player._y = y;
                }
                break;
            case 2:
                g_deletestar.push_back({ 2, id, 0, 0 });
                break;
            case 3:
                // 기존 유저 위치 수정
                for (auto& starmove : g_starmove)
                {
                    if (starmove._id == id)
                    {
                        starmove._x = x;
                        starmove._y = y;
                        moveAdd = false;
                        break;
                    }
                }

                // 신규 유저 들어왔다면...
                if (moveAdd)
                {
                    g_starmove.push_back({ 3, id, x, y });
                }
                break;
            }

            sum += 16;
        }
    }

    // 다른 유저 접속 종료 시 삭제
    for (auto iter = g_deletestar.begin(); iter != g_deletestar.end();)
    {
        bool isDelete = false;
        for (auto smIter = g_starmove.begin(); smIter != g_starmove.end();)
        {
            if (smIter->_id == iter->_id)
            {
                smIter = g_starmove.erase(smIter);
                isDelete = true;
                break;
            }
            else
            {
               ++smIter;
            }
        }

        if (isDelete)
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

int Render()
{
    if (g_player._x == -1)
    {
        return 1;
    }

    Buffer_Clear();

    for (auto& starmove : g_starmove)
    {
        Sprite_Draw(starmove._x, starmove._y, '*');
    }

    Buffer_Flip();

    return 1;
}*/