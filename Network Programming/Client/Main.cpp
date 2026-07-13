#define _CRT_SECURE_NO_WARNINGS
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
    _setmode(_fileno(stdin), _O_U16TEXT);
    timeBeginPeriod(1);

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        return 1;
    }

    SOCKADDR_IN sockaddrin;
    memset(&sockaddrin, 0, sizeof(sockaddrin));
    sockaddrin.sin_family = AF_INET;
    sockaddrin.sin_port = htons(SERVERPORT);
    InetPton(AF_INET, SERVERIP, &sockaddrin.sin_addr);

    int retval = connect(sock, (SOCKADDR*)&sockaddrin, sizeof(sockaddrin));
    if (retval == SOCKET_ERROR)
    {
        return 1;
    }

    wchar_t buf[BUFSIZE];
    int len;

    while (true)
    {
        if (fgetws(buf, BUFSIZE, stdin) == NULL)
        {
            break;
        }

        int length = static_cast<int>(wcslen(buf));
        if (buf[length - 1] == '\n')
        {
            buf[length - 1] = '\0';
        }

        if (wcslen(buf) == 0)
        {
            break;
        }

        len = static_cast<int>(wcslen(buf) * sizeof(wchar_t));
        retval = send(sock, (char*)&len, sizeof(int), 0);
        if (retval == SOCKET_ERROR)
        {
            wprintf(L"%d\n", WSAGetLastError());
            break;
        }

        retval = send(sock, (char*)buf, len, 0);
        if (retval == SOCKET_ERROR)
        {
            wprintf(L"%d\n", WSAGetLastError());
            break;
        }

        wchar_t recvMessage[BUFSIZE + 1];
        int recvlen;
        retval = recv(sock, (char*)&recvlen, sizeof(int), MSG_WAITALL);
        if (retval == SOCKET_ERROR)
        {
            wprintf(L"%d\n", WSAGetLastError());
            break;
        }
        else if (retval == 0)
        {
            break;
        }

        retval = recv(sock, (char*)recvMessage, recvlen, MSG_WAITALL);
        if (retval == SOCKET_ERROR)
        {
            wprintf(L"%d\n", WSAGetLastError());
            break;
        }
        else if (retval == 0)
        {
            break;
        }

        recvMessage[retval / sizeof(wchar_t)] = L'\0';
        wprintf(L"[TCP/%s:%d] %s\n", SERVERIP, SERVERPORT, recvMessage);
    }

    closesocket(sock);
    WSACleanup();

    return 0;
}