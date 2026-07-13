// Server
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
#define BUFSIZE     50

int wmain(int argc, WCHAR* argv[])
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

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
            int len = static_cast<int>(BUFSIZE * sizeof(wchar_t));

            retval = recv(client_sock, (char*)buf, len, MSG_WAITALL);
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

            wchar_t sendMessage[BUFSIZE];

            if (fgetws(sendMessage, BUFSIZE, stdin) == NULL)
            {
                break;
            }

            int length = static_cast<int>(wcslen(sendMessage));
            if (sendMessage[length - 1] == '\n')
            {
                sendMessage[length - 1] = '\0';
            }

            if (wcslen(sendMessage) == 0)
            {
                break;
            }

            retval = send(client_sock, (char*)sendMessage, len, 0);
            if (retval == SOCKET_ERROR)
            {
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

// Client
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
#define BUFSIZE     50

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

    wchar_t buf[BUFSIZE];

    while (true)
    {
        int len = static_cast<int>(BUFSIZE * sizeof(wchar_t));

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

        retval = send(sock, (char*)buf, len, 0);
        if (retval == SOCKET_ERROR)
        {
            break;
        }

        wchar_t recvMessage[BUFSIZE + 1];

        retval = recv(sock, (char*)recvMessage, len, MSG_WAITALL);
        if (retval == SOCKET_ERROR)
        {
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
}*/