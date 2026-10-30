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

uint64_t g_count = 100000;

SOCKET sock;
const char* buf[4] = { "Hello", "MyName", "IsMinseok", "Bye~" };

bool update();

int wmain(int argc, WCHAR* argv[])
{
    timeBeginPeriod(1);
    srand((uint32_t)time(nullptr));

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 1;
    }

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

    while (g_count--)
    {
        if (!update())
        {
            break;
        }
    }

    closesocket(sock);

    WSACleanup();

    return 0;
}

bool update()
{
    int index = rand() % 4;
    const char* bbuf = buf[index];
    int len = strlen(buf[index]);

    auto sendRet = send(sock, bbuf, len, 0);
    if (sendRet == SOCKET_ERROR)
    {
        printf("send : %d", WSAGetLastError());
        return false;
    }

    char rbuf[BUFSIZE];
    auto recvRet = recv(sock, rbuf, sendRet, 0);
    if (recvRet == SOCKET_ERROR)
    {
        printf("send : %d", WSAGetLastError());
        return false;
    }
    else if (recvRet == 0)
    {
        return false;
    }

    return true;
}