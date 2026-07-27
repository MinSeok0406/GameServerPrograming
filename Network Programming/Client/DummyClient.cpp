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
#define CLIENT      40

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

Player g_player[CLIENT];
list<IDALLOC>   g_idalloc[CLIENT];
list<CREATESTAR> g_createstar[CLIENT];
list<DELETESTAR> g_deletestar[CLIENT];
list<STARMOVE>  g_starmove[CLIENT];

fd_set rset;
timeval t;
SOCKET server_sock[CLIENT];
int recvByte;
int useTime;
DWORD tm;
char buf[BUFSIZE];

int inputMove(int index);
int networkLogic(int index);
int Render(int index);

int wmain(int argc, WCHAR* argv[])
{
    timeBeginPeriod(1);
    srand(unsigned int(time(nullptr)));
    cs_Initial();

    for (auto i = 0; i < CLIENT; ++i)
    {
        memset(&g_player[i], -1, sizeof(g_player[i]));
    }

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

    for (auto i = 0; i < CLIENT; ++i)
    {
        server_sock[i] = socket(AF_INET, SOCK_STREAM, 0);
        if (server_sock[i] == INVALID_SOCKET)
        {
            wprintf(L"%d\n", WSAGetLastError());
            return 1;
        }

        SOCKADDR_IN serveraddr;
        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(SERVERPORT);
        InetPton(AF_INET, SERVERIP, &serveraddr.sin_addr);

        int connectRet = connect(server_sock[i], (SOCKADDR*)&serveraddr, sizeof(serveraddr));
        if (connectRet == SOCKET_ERROR)
        {
            wprintf(L"%d\n", WSAGetLastError());
            return 1;
        }

        u_long on = 1;
        int nonblkRet = ioctlsocket(server_sock[i], FIONBIO, &on);
        if (nonblkRet == SOCKET_ERROR)
        {
            wprintf(L"%d\n", WSAGetLastError());
            return 1;
        }
    }

    tm = timeGetTime();

    while (true)
    {
        for (auto i = 0; i < CLIENT; ++i)
        {
            if (!inputMove(i))
            {
                break;
            }

            if (!networkLogic(i))
            {
                break;
            }
        }

        Render(39);

        useTime = (int)(timeGetTime() - tm);
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

    for (auto i = 0; i < CLIENT; ++i)
    {
        closesocket(server_sock[i]);
    }
    
    WSACleanup();

    return 0;
}

int inputMove(int index)
{
    if (g_player[index]._id == -1)
    {
        return 1;
    }

    int randNum = rand() % 15;
    bool flag = false;
    if (randNum == 0)
    {
        if (g_player[index]._x - 1 == -1 || g_player[index]._y - 1 == -1)
        {
            return 1;
        }

        g_player[index]._x -= 1;
        g_player[index]._y -= 1;
        flag = true;
    }
    else if (randNum == 2)
    {
        if (g_player[index]._x + 1 == dfSCREEN_WIDTH - 1 || g_player[index]._y - 1 == -1)
        {
            return 1;
        }

        g_player[index]._x += 1;
        g_player[index]._y -= 1;
        flag = true;
    }
    else if (randNum == 4)
    {
        if (g_player[index]._x + 1 == dfSCREEN_WIDTH - 1 || g_player[index]._y + 1 == dfSCREEN_HEIGHT)
        {
            return 1;
        }

        g_player[index]._x += 1;
        g_player[index]._y += 1;
        flag = true;
    }
    else if (randNum == 7)
    {
        if (g_player[index]._x - 1 == -1 || g_player[index]._y + 1 == dfSCREEN_HEIGHT)
        {
            return 1;
        }

        g_player[index]._x -= 1;
        g_player[index]._y += 1;
        flag = true;
    }
    else if (randNum == 9)
    {
        if (g_player[index]._x - 1 == -1)
        {
            return 1;
        }

        g_player[index]._x -= 1;
        flag = true;
    }
    else if (randNum == 11)
    {
        if (g_player[index]._y - 1 == -1)
        {
            return 1;
        }

        g_player[index]._y -= 1;
        flag = true;
    }
    else if (randNum == 13)
    {
        if (g_player[index]._x + 1 == dfSCREEN_WIDTH - 1)
        {
            return 1;
        }

        g_player[index]._x += 1;
        flag = true;
    }
    else if (randNum == 14)
    {
        if (g_player[index]._y + 1 == dfSCREEN_HEIGHT)
        {
            return 1;
        }

        g_player[index]._y += 1;
        flag = true;
    }

    if (flag)
    {
        // send
        for (auto& starmove : g_starmove[index])
        {
            if (starmove._id == g_player[index]._id)
            {
                starmove._x = g_player[index]._x;
                starmove._y = g_player[index]._y;

                // [수정] server_sock[CLIENT] -> server_sock[index]
                // CLIENT(40)는 배열 크기일 뿐 유효 인덱스가 아니라서 배열 범위를 벗어난 접근이었음
                int sendRet = send(server_sock[index], (char*)&starmove, sizeof(STARMOVE), 0);
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

int networkLogic(int index)
{
    FD_ZERO(&rset);
    FD_SET(server_sock[index], &rset);
    t.tv_sec = 0;
    t.tv_usec = 1000;

    int selectRet = select(0, &rset, NULL, NULL, &t);
    if (selectRet == SOCKET_ERROR)
    {
        wprintf(L"%d\n", WSAGetLastError());
        return 0;
    }

    if (FD_ISSET(server_sock[index], &rset))
    {
        int recvRet = recv(server_sock[index], buf, sizeof(buf), 0);
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
                g_player[index]._id = id;
                g_idalloc[index].push_back({ 0, id, 0, 0 });
                break;
            case 1:
                // 내 캐릭터 + 다른 유저 위치 받아오기
                g_createstar[index].push_back({ 1, id, x, y });
                g_starmove[index].push_back({ 3, id, x, y });
                if (id == g_player[index]._id)
                {
                    g_player[index]._x = x;
                    g_player[index]._y = y;
                }
                break;
            case 2:
                g_deletestar[index].push_back({ 2, id, 0, 0 });
                break;
            case 3:
                // 기존 유저 위치 수정
                for (auto& starmove : g_starmove[index])
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
                    g_starmove[index].push_back({ 3, id, x, y });
                }
                break;
            }

            sum += 16;
        }
    }

    // 다른 유저 접속 종료 시 삭제
    for (auto iter = g_deletestar[index].begin(); iter != g_deletestar[index].end();)
    {
        bool isDelete = false;
        for (auto smIter = g_starmove[index].begin(); smIter != g_starmove[index].end();)
        {
            if (smIter->_id == iter->_id)
            {
                smIter = g_starmove[index].erase(smIter);
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
            iter = g_deletestar[index].erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    return 1;
}

int Render(int index)
{
    if (g_player[index]._x == -1)
    {
        return 1;
    }

    Buffer_Clear();

    for (auto& starmove : g_starmove[index])
    {
        Sprite_Draw(starmove._x, starmove._y, '*');
    }

    Buffer_Flip();

    return 1;
}*/