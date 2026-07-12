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
        return 1;
    }

    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR)
    {
        return 1;
    }

    SOCKET client_sock;
    sockaddr_in clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];

    while (1)
    {
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (sockaddr*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET)
        {
            wprintf(L"accept fail\n");
            break;
        }

        wchar_t addr[INET_ADDRSTRLEN];
        InetNtop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
        wprintf(L"\n[TCP 서버] 클라이언트 접속: IP 주소 = %s, 포트 번호 = %d\n", addr, ntohs(clientaddr.sin_port));

        while (1)
        {
            char recvbuf[4];
            int received = 0;
            bool ok = true;

            while (received < 4)
            {
                retval = recv(client_sock, recvbuf + received, 4 - received, 0);
                if (retval == SOCKET_ERROR)
                {
                    wprintf(L"recv fail\n");
                    ok = false;
                    break;
                }
                else if (retval == 0)
                {
                    ok = false;
                    break;
                }

                received += retval;
            }

            if (!ok)
            {
                break;
            }

            int32_t an;
            memcpy(&an, recvbuf, 4);
            int a = ntohl(an);

            char str[33];
            int cnt = 31;
            unsigned int au = (unsigned int)a;
            while (cnt >= 0)
            {
                str[31 - cnt] = ((au >> cnt) & 1) ? '1' : '0';
                cnt--;
            }
            str[32] = '\0';

            wprintf(L"[TCP/%s:%d] %d = %hs\n", addr, ntohs(clientaddr.sin_port), a, str);

            retval = send(client_sock, str, 32, 0);
            if (retval == SOCKET_ERROR)
            {
                wprintf(L"send fail\n");
                break;
            }
        }

        closesocket(client_sock);
        wprintf(L"[TCP 서버] 클라이언트 종료 : IP 주소 = %s, 포트 번호 = %d\n", addr, ntohs(clientaddr.sin_port));
    }

    closesocket(listen_sock);
    WSACleanup();

    return 0;
}*/