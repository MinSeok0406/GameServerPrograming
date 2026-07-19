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
    _setmode(_fileno(stdin), _O_U16TEXT);
    timeBeginPeriod(1);

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET)
    {
        return 1;
    }

    SOCKADDR_IN serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    InetPton(AF_INET, SERVERIP, &serveraddr.sin_addr);
    serveraddr.sin_port = htons(SERVERPORT);

    SOCKADDR_IN peeraddr;
    int addrlen;
    wchar_t buf[BUFSIZE + 1];
    int len;

    while (true)
    {
        wprintf(L"\n[보낼 데이터] ");
        if (fgetws(buf, BUFSIZE + 1, stdin) == NULL)
        {
            break;
        }

        len = (int)wcslen(buf);
        if (buf[len - 1] == '\n')
        {
            buf[len - 1] = '\0';
        }

        if (wcslen(buf) == 0)
        {
            break;
        }

        int retval = sendto(sock, (char*)buf, (int)(wcslen(buf) * sizeof(wchar_t)), 0, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
        if (retval == SOCKET_ERROR)
        {
            wprintf(L"%d\n", WSAGetLastError());
            return 1;
        }
        wprintf(L"[UDP 클라이언트] %d바이트를 보냈습니다.\n", retval);

        addrlen = sizeof(peeraddr);
        retval = recvfrom(sock, (char*)buf, BUFSIZE, 0, (SOCKADDR*)&peeraddr, &addrlen);
        if (retval == SOCKET_ERROR)
        {
            wprintf(L"%d\n", WSAGetLastError());
            return 1;
        }

        if (memcmp(&peeraddr, &serveraddr, sizeof(peeraddr)))
        {
            wprintf(L"[오류] 잘못된 데이터입니다.\n");
            break;
        }

        buf[retval / sizeof(wchar_t)] = '\0';
        wprintf(L"[UDP 클라이언트] %d바이트를 받았습니다.\n", retval);
        wprintf(L"[받은 데이터] %s\n", buf);
    }

    closesocket(sock);
    WSACleanup();

    return 0;
}*/