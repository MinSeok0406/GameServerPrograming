// 서버
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

int _recv_ahead(SOCKET s, wchar_t* p)
{
    __declspec(thread) static int nbytes = 0;
    __declspec(thread) static wchar_t buf[1024];
    __declspec(thread) static wchar_t* ptr;

    int len = static_cast<int>(1024 * sizeof(wchar_t));

    if (nbytes == 0 || nbytes == SOCKET_ERROR)
    {
        nbytes = recv(s, (char*)buf, len, 0);
        if (nbytes == SOCKET_ERROR)
        {
            return SOCKET_ERROR;
        }
        else if (nbytes == 0)
        {
            return 0;
        }
        ptr = buf;
    }

    nbytes -= sizeof(wchar_t);
    *p = *ptr++;
    return 1;
}

int recvline(SOCKET s, wchar_t* buf, int maxlen)
{
    int n, nbytes;
    wchar_t c;
    wchar_t* ptr = buf;

    for (n = 1; n < maxlen; ++n)
    {
        nbytes = _recv_ahead(s, &c);
        if (nbytes == 1)
        {
            *ptr++ = c;
            if (c == L'\n')
            {
                break;
            }
        }
        else if (nbytes == 0)
        {
            *ptr = 0;
            return n - 1;
        }
        else
        {
            return SOCKET_ERROR;
        }
    }

    *ptr = 0;
    return n;
}

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
            retval = recvline(client_sock, buf, BUFSIZE + 1);
            if (retval == SOCKET_ERROR)
            {
                break;
            }
            else if (retval == 0)
            {
                break;
            }

            wprintf(L"[TCP/%s:%d] %s", addr, ntohs(clientaddr.sin_port), buf);

            wchar_t sendMessage[BUFSIZE];
            if (fgetws(sendMessage, BUFSIZE, stdin) == NULL)
            {
                break;
            }

            int len = static_cast<int>(wcslen(sendMessage) * sizeof(wchar_t));
            retval = send(client_sock, (char*)sendMessage, len, 0);
            if (retval == SOCKET_ERROR)
            {
                wprintf(L"%d\n", WSAGetLastError());
                break;
            }
        }

        closesocket(client_sock);
        wprintf(L"[TCP 서버] 클라이언트 종료 : IP 주소 = %s, 포트 번호 = %d\n", addr, ntohs(clientaddr.sin_port));
    }

    closesocket(listen_sock);
    WSACleanup();

    return 0;
}

클라이언트
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
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

int _recv_ahead(SOCKET s, wchar_t* p)
{
    __declspec(thread) static int nbytes = 0;
    __declspec(thread) static wchar_t buf[1024];
    __declspec(thread) static wchar_t* ptr;

    int len = static_cast<int>(1024 * sizeof(wchar_t));

    if (nbytes == 0 || nbytes == SOCKET_ERROR)
    {
        nbytes = recv(s, (char*)buf, len, 0);
        if (nbytes == SOCKET_ERROR)
        {
            return SOCKET_ERROR;
        }
        else if (nbytes == 0)
        {
            return 0;
        }
        ptr = buf;
    }

    nbytes -= sizeof(wchar_t);
    *p = *ptr++;
    return 1;
}

int recvline(SOCKET s, wchar_t* buf, int maxlen)
{
    int n, nbytes;
    wchar_t c;
    wchar_t* ptr = buf;

    for (n = 1; n < maxlen; ++n)
    {
        nbytes = _recv_ahead(s, &c);
        if (nbytes == 1)
        {
            *ptr++ = c;
            if (c == L'\n')
            {
                break;
            }
        }
        else if (nbytes == 0)
        {
            *ptr = 0;
            return n - 1;
        }
        else
        {
            return SOCKET_ERROR;
        }
    }

    *ptr = 0;
    return n;
}

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

    while (true)
    {
        if (fgetws(buf, BUFSIZE, stdin) == NULL)
        {
            break;
        }

        int len = static_cast<int>(wcslen(buf) * sizeof(wchar_t));
        retval = send(sock, (char*)buf, len, 0);
        if (retval == SOCKET_ERROR)
        {
            break;
        }

        wchar_t recvMessage[BUFSIZE + 1];
        retval = recvline(sock, recvMessage, BUFSIZE + 1);
        if (retval == SOCKET_ERROR)
        {
            break;
        }
        else if (retval == 0)
        {
            break;
        }

        wprintf(L"[TCP/%s:%d] %s", SERVERIP, SERVERPORT, recvMessage);
    }

    closesocket(sock);
    WSACleanup();

    return 0;
}*/
