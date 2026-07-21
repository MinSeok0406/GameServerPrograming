/*#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <time.h>
#include <fcntl.h>
#include <io.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <string>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

#define SERVERPORT  47000
#define BUFSIZE     512

struct SOCKETINFO
{
    SOCKET sock;
    char buf[BUFSIZE + 1];
    int recvbytes;
    int sendbytes;
};

int nTotalSockets = 0;
SOCKETINFO* SocketInfoArray[FD_SETSIZE];

bool AddSocketInfo(SOCKET sock);
void RemoveSocketInfo(int nIndex);

int wmain(int argc, WCHAR* argv[])
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    timeBeginPeriod(1);
    srand(unsigned int(time(nullptr)));

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 1;
    }

    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET)
    {
        wprintf(L"%d\n", WSAGetLastError());
        return 1;
    }

    sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    int retval = bind(listen_sock, (sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR)
    {
        wprintf(L"%d\n", WSAGetLastError());
        return 1;
    }

    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR)
    {
        wprintf(L"%d\n", WSAGetLastError());
        return 1;
    }

    u_long on = 1;
    retval = ioctlsocket(listen_sock, FIONBIO, &on);
    if (retval == SOCKET_ERROR)
    {
        wprintf(L"%d\n", WSAGetLastError());
        return 1;
    }

    fd_set rset;
    fd_set wset;
    int nready = 5;
    SOCKET client_sock;
    sockaddr_in clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];

    while (1)
    {
        FD_ZERO(&rset);
        FD_ZERO(&wset);
        FD_SET(listen_sock, &rset);
        for (auto i = 0; i < nTotalSockets; ++i)
        {
            if (SocketInfoArray[i]->recvbytes > SocketInfoArray[i]->sendbytes)
            {
                FD_SET(SocketInfoArray[i]->sock, &wset);
            }
            else
            {
                FD_SET(SocketInfoArray[i]->sock, &rset);
            }
        }

        retval = select(0, &rset, &wset, nullptr, nullptr);
        if (retval == SOCKET_ERROR)
        {
            wprintf(L"%d\n", WSAGetLastError());
            break;
        }

        if (FD_ISSET(listen_sock, &rset))
        {
            addrlen = sizeof(clientaddr);
            client_sock = accept(listen_sock, (sockaddr*)&clientaddr, &addrlen);
            if (client_sock == INVALID_SOCKET)
            {
                wprintf(L"accept fail\n");
                break;
            }
            else
            {
                wchar_t addr[INET_ADDRSTRLEN];
                InetNtop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
                wprintf(L"\n[TCP 서버] 클라이언트 접속: IP 주소 = %s, 포트 번호 = %d\n", addr, ntohs(clientaddr.sin_port));

                if (!AddSocketInfo(client_sock))
                {
                    closesocket(client_sock);
                }
            }

            if (--nready <= 0)
            {
                continue;
            }
        }

        for (auto i = 0; i < nTotalSockets; ++i)
        {
            SOCKETINFO* ptr = SocketInfoArray[i];
            if (FD_ISSET(ptr->sock, &rset))
            {
                retval = recv(ptr->sock, ptr->buf, BUFSIZE, 0);
                if (retval == SOCKET_ERROR)
                {
                    wprintf(L"%d\n", WSAGetLastError());
                    RemoveSocketInfo(i);
                }
                else if (retval == 0)
                {
                    RemoveSocketInfo(i);
                }
                else
                {
                    ptr->recvbytes = retval;
                    addrlen = sizeof(clientaddr);
                    getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addrlen);

                    ptr->buf[ptr->recvbytes] = '\0';
                    wchar_t addr[INET_ADDRSTRLEN];
                    InetNtop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
                    wprintf(L"[TCP/%s:%d] %hs\n", addr, ntohs(clientaddr.sin_port), ptr->buf);
                }
            }
            else if (FD_ISSET(ptr->sock, &wset))
            {
                retval = send(ptr->sock, ptr->buf + ptr->sendbytes, ptr->recvbytes - ptr->sendbytes, 0);
                if (retval == SOCKET_ERROR)
                {
                    wprintf(L"%d\n", WSAGetLastError());
                    RemoveSocketInfo(i);
                }
                else
                {
                    ptr->sendbytes += retval;
                    if (ptr->recvbytes == ptr->sendbytes)
                    {
                        ptr->recvbytes = ptr->sendbytes = 0;
                    }
                }
            }
        }
    }

    closesocket(listen_sock);
    WSACleanup();

    return 0;
}

bool AddSocketInfo(SOCKET sock)
{
    if (nTotalSockets >= FD_SETSIZE)
    {
        wprintf(L"[오류] 소켓 정보를 추가할 수 없습니다.\n");
        return false;
    }

    SOCKETINFO* ptr = new SOCKETINFO;
    if (ptr == NULL)
    {
        return false;
    }

    ptr->sock = sock;
    ptr->recvbytes = 0;
    ptr->sendbytes = 0;
    SocketInfoArray[nTotalSockets++] = ptr;

    return true;
}

void RemoveSocketInfo(int nIndex)
{
    SOCKETINFO* ptr = SocketInfoArray[nIndex];

    SOCKADDR_IN clientaddr;
    int addrlen = sizeof(clientaddr);
    getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addrlen);

    wchar_t addr[INET_ADDRSTRLEN];
    InetNtop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
    wprintf(L"[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n", addr, ntohs(clientaddr.sin_port));

    closesocket(ptr->sock);
    delete ptr;

    if (nIndex != (nTotalSockets - 1))
    {
        SocketInfoArray[nIndex] = SocketInfoArray[nTotalSockets - 1];
        --nTotalSockets;
    }
}*/
