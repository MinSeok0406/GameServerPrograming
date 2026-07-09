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
    timeBeginPeriod(1);

    if (argc > 1)
    {
        SERVERIP = argv[1];
    }

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

    sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    InetPton(AF_INET, SERVERIP, &serveraddr.sin_addr);
    serveraddr.sin_port = htons(SERVERPORT);
    
    int retval = connect(sock, (sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR)
    {
        return 1;
    }

    char buf[BUFSIZE + 1];
    int len;

    while (1)
    {
        wprintf(L"\n[보낼 데이터] ");
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

        retval = send(sock, buf, (int)strlen(buf), 0);
        if (retval == SOCKET_ERROR)
        {
            wprintf(L"send fail\n");
            break;
        }

        wprintf(L"[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

        retval = recv(sock, buf, retval, MSG_WAITALL);
        if (retval == SOCKET_ERROR)
        {
            wprintf(L"recv fail\n");
            break;
        }
        else if (retval == 0)
        {
            break;
        }

        buf[retval] = '\0';
        wprintf(L"[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);
        wprintf(L"[받은 데이터] %hs\n", buf);
    }

    closesocket(sock);
    
    WSACleanup();

    return 0;
}
