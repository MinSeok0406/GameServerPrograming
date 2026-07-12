/*#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <time.h>
#include <fcntl.h>
#include <io.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

const wchar_t* SERVERIP = L"127.0.0.1";
#define SERVERPORT  47000
#define BUFSIZE     512

int wmain(int argc, WCHAR* argv[])
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    timeBeginPeriod(1);

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 1;
    }

    SOCKET sock;
    int retval;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        return 1;
    }

    sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    InetPton(AF_INET, SERVERIP, &serveraddr.sin_addr);
    serveraddr.sin_port = htons(SERVERPORT);

    retval = connect(sock, (sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR)
    {
        return 1;
    }

    char buf[BUFSIZE + 1];
    int a;

    while (1)
    {
        wprintf(L"\n[보낼 데이터] ");
        if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
        {
            break;
        }

        if (sscanf(buf, "%d", &a) != 1)
        {
            continue;
        }

        char sendbuf[4];
        int an = htonl(a);
        memcpy(sendbuf, &an, 4);

        retval = send(sock, sendbuf, sizeof(sendbuf), 0);
        if (retval == SOCKET_ERROR)
        {
            wprintf(L"send fail\n");
            break;
        }

        char recvbuf[33];
        int received = 0;
        bool ok = true;

        while (received < 32)
        {
            retval = recv(sock, recvbuf + received, 32 - received, 0);
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

        recvbuf[32] = '\0';
        wprintf(L"[받은 데이터] %hs\n", recvbuf);
    }

    closesocket(sock);

    WSACleanup();

    return 0;
}*/
