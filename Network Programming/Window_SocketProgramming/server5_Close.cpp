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
#define BUFSIZE     1024

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

    SOCKADDR_IN serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERVERPORT);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
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
    SOCKADDR_IN clientaddr;
    int addrlen;
    wchar_t buf[BUFSIZE + 1];

    while (true)
    {
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (sockaddr*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET)
        {
            break;
        }

        wchar_t addr[INET_ADDRSTRLEN];
        InetNtop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
        wprintf(L"\n[TCP 서버] 클라이언트 접속 : IP 주소 = %s, 포트 번호 = %d\n", addr, ntohs(clientaddr.sin_port));

        while (true)
        {
            retval = recv(client_sock, (char*)buf, BUFSIZE * sizeof(wchar_t), MSG_WAITALL);
            if (retval == SOCKET_ERROR)
            {
                break;
            }
            else if (retval == 0)
            {
                break;
            }

            buf[retval / sizeof(wchar_t)] = L'\0';
            wprintf(L"[TCP/%s:%d] %s\n", addr, ntohs(clientaddr.sin_port), buf);
        }

        closesocket(client_sock);
        wprintf(L"[TCP 서버] 클라이언트 종료 : IP 주소 = %s, 포트 번호 = %d\n", addr, ntohs(clientaddr.sin_port));
    }

    closesocket(listen_sock);
    WSACleanup();

    return 0;
}*/