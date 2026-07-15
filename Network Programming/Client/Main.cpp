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
#define BUFSIZE     500

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

    SOCKET sock;
    int retval;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        return 1;
    }

    SOCKADDR_IN sockaddrin;
    memset(&sockaddrin, 0, sizeof(sockaddrin));
    sockaddrin.sin_family = AF_INET;
    sockaddrin.sin_port = htons(SERVERPORT);
    InetPton(AF_INET, SERVERIP, &sockaddrin.sin_addr);

    retval = connect(sock, (SOCKADDR*)&sockaddrin, sizeof(sockaddrin));
    if (retval == SOCKET_ERROR)
    {
        return 1;
    }

    FILE* fp;
    wchar_t buf[BUFSIZE];

    _wfopen_s(&fp, L"butterfly.png", L"r");
    if (fp == NULL)
    {
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    auto totalsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    while (true)
    {
        if (totalsize <= 0)
        {
            break;
        }

        int length = min(totalsize, BUFSIZE * sizeof(wchar_t));
        fread(buf, 1, length, fp);
        totalsize -= length;

        retval = send(sock, (char*)buf, length, 0);
        if (retval == SOCKET_ERROR)
        {
            break;
        }
    }

    fclose(fp);
    closesocket(sock);
    WSACleanup();

    return 0;
}