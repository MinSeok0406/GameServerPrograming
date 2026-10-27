#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <time.h>
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
    int len;

    while (1)
    {
        printf("[보낼 데이터] ");
        if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
        {
            break;
        }

        len = (int)strlen(buf);
        if (buf[len - 1] == '\n')
        {
            buf[len - 1] = '\0';
        }
        
        if (strlen(buf) == 0)
        {
            break;
        }

        int sendRet = send(sock, buf, (int)strlen(buf), 0);
        if (sendRet == SOCKET_ERROR)
        {
            break;
        }

        int recvRet = recv(sock, buf, sendRet, MSG_WAITALL);
        if (recvRet == SOCKET_ERROR)
        {
            break;
        }
        else if (recvRet == 0)
        {
            break;
        }

        buf[recvRet] = '\0';
        printf("[받은 데이터] %s\n", buf);
    }

    closesocket(sock);

    WSACleanup();

    return 0;
}
