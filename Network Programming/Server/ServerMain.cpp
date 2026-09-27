#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <time.h>
#include <process.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <string>
#include <list>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

#define SERVERPORT 47000
#define BUFSIZE 256

struct SOCKETINFO
{
    SOCKET sock;
    char buf[BUFSIZE + 1];
    int recvbytes;
    bool isExit;
};

list<SOCKETINFO*> g_clientInfo;
SOCKET g_listensock;

bool netLogic();

bool addSocketInfo(SOCKET sock);
bool removeSocketInfo();

int wmain()
{
    timeBeginPeriod(1);
    srand((uint32_t)time(nullptr));

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 1;
    }

    g_listensock = socket(AF_INET, SOCK_STREAM, 0);
    if (g_listensock == INVALID_SOCKET)
    {
        printf("socket : %d\n", WSAGetLastError());
        return 1;
    }
    
    linger lg;
    lg.l_onoff = 1;
    lg.l_linger = 0;
    setsockopt(g_listensock, SOL_SOCKET, SO_LINGER, (const char*)&lg, sizeof(lg));

    SOCKADDR_IN serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    auto bindRet = bind(g_listensock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (bindRet == SOCKET_ERROR)
    {
        printf("bind : %d\n", WSAGetLastError());
        return 1;
    }

    u_long on = 1;
    auto ioctRet = ioctlsocket(g_listensock, FIONBIO, &on);
    if (ioctRet == SOCKET_ERROR)
    {
        printf("ioct : %d\n", WSAGetLastError());
        return 1;
    }

    auto listenRet = listen(g_listensock, SOMAXCONN);
    if (listenRet == SOCKET_ERROR)
    {
        printf("listen : %d\n", WSAGetLastError());
        return 1;
    }

    while (true)
    {
        netLogic();
        removeSocketInfo();
    }

    closesocket(g_listensock);
    WSACleanup();

    return 0;
}

bool netLogic()
{
    fd_set rset;
    SOCKET clientsocket;
    SOCKADDR_IN clientaddr;
    int addrlen = sizeof(clientaddr);

    FD_ZERO(&rset);

    FD_SET(g_listensock, &rset);
    for (auto& client : g_clientInfo)
    {
        FD_SET(client->sock, &rset);
    }

    auto selectRet = select(0, &rset, NULL, NULL, NULL);
    if (selectRet == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAEWOULDBLOCK)
        {
            printf("select : %d\n", WSAGetLastError());
        }

        return false;
    }

    if (FD_ISSET(g_listensock, &rset))
    {
        clientsocket = accept(g_listensock, (SOCKADDR*)&clientaddr, &addrlen);
        if (clientsocket == INVALID_SOCKET)
        {
            printf("accept : %d\n", WSAGetLastError());
            return false;
        }

        wchar_t addr[INET_ADDRSTRLEN];
        InetNtop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
        printf("\n[TCP/IPv4] Client Accept : IP addr : %s, Port : %d\n",
            addr, ntohs(clientaddr.sin_port));

        if (!addSocketInfo(clientsocket))
        {
            closesocket(clientsocket);
        }
    }

    for (auto it = g_clientInfo.begin(); it != g_clientInfo.end(); ++it)
    {
        SOCKETINFO* ptr = *it;
        if (FD_ISSET(ptr->sock, &rset))
        {
            auto recvRet = recv(ptr->sock, ptr->buf + ptr->recvbytes, BUFSIZE - ptr->recvbytes, 0);
            if (recvRet == SOCKET_ERROR)
            {
                if (WSAGetLastError() != WSAEWOULDBLOCK)
                {
                    printf("recv : %d\n", WSAGetLastError());
                }

                ptr->isExit = true;
                continue;
            }
            else if (recvRet == 0)
            {
                ptr->isExit = true;
                continue;
            }

            ptr->recvbytes += recvRet;

            if (ptr->recvbytes == BUFSIZE)
            {
                ptr->recvbytes = 0;
                for (auto it2 = g_clientInfo.begin(); it2 != g_clientInfo.end(); ++it2)
                {
                    SOCKETINFO* ptr2 = *it2;
                    if (ptr->sock != ptr2->sock)
                    {
                        auto sendRet = send(ptr2->sock, ptr2->buf, BUFSIZE, 0);
                        if (sendRet == SOCKET_ERROR)
                        {
                            ptr2->isExit = true;
                            continue;
                        }
                    }
                }
            }
        }
    }


    return true;
}

bool addSocketInfo(SOCKET sock)
{
    if (g_clientInfo.size() >= FD_SETSIZE)
    {
        return false;
    }

    SOCKETINFO* ptr = new SOCKETINFO;
    if (ptr == nullptr)
    {
        return false;
    }

    ptr->sock = sock;
    ptr->recvbytes = 0;
    ptr->isExit = false;
    g_clientInfo.push_back(ptr);

    return true;
}

bool removeSocketInfo()
{
    for (auto it = g_clientInfo.begin(); it != g_clientInfo.end();)
    {
        SOCKETINFO* ptr = *it;
        if (ptr->isExit == true)
        {
            // 클라이언트 정보 출력...

            closesocket(ptr->sock);
            delete ptr;
            it = g_clientInfo.erase(it);
        }
        else
        {
            ++it;
        }
    }

    return true;
}
