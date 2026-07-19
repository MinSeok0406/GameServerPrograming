/*#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <time.h>
#include <fcntl.h>
#include <io.h>
#include <process.h>
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

struct ClientAddr
{
    SOCKET s;
    SOCKADDR_IN sockin;
};

unsigned int WINAPI processClient(LPVOID arg)
{
    ClientAddr* ca = (ClientAddr*)arg;

    SOCKET client_sock = ca->s;
    int retval;
    SOCKADDR_IN clientaddr = ca->sockin;
    wchar_t addr[INET_ADDRSTRLEN];
    InetNtop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
    wchar_t buf[BUFSIZE + 1];

    delete ca;

    while (true)
    {
        retval = recv(client_sock, (char*)buf, BUFSIZE, 0);
        if (retval == SOCKET_ERROR)
        {
            wprintf(L"%d\n", WSAGetLastError());
            break;
        }
        else if (retval == 0)
        {
            break;
        }

        buf[retval / sizeof(wchar_t)] = L'\0';
        wprintf(L"[TCP/%s:%d] %s\n", addr, ntohs(clientaddr.sin_port), buf);

        retval = send(client_sock, (char*)buf, retval, 0);
        if (retval == SOCKET_ERROR)
        {
            wprintf(L"%d\n", WSAGetLastError());
            break;
        }
    }

    closesocket(client_sock);
    wprintf(L"[TCP 서버] 클라이언트 종료: IP 주소 = %s, 포트 번호 = %d\n", addr, ntohs(clientaddr.sin_port));
    return 0;
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

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        wprintf(L"%d\n", WSAGetLastError());
        return 1;
    }

    SOCKADDR_IN serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERVERPORT);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int retval = bind(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR)
    {
        wprintf(L"%d\n", WSAGetLastError());
        return 1;
    }

    retval = listen(sock, SOMAXCONN);
    if (retval == SOCKET_ERROR)
    {
        wprintf(L"%d\n", WSAGetLastError());
        return 1;
    }

    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;
    HANDLE hThread;

    while (true)
    {
        addrlen = sizeof(clientaddr);
        client_sock = accept(sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == SOCKET_ERROR)
        {
            wprintf(L"%d\n", WSAGetLastError());
            break;
        }

        wchar_t addr[INET_ADDRSTRLEN];
        InetNtop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
        wprintf(L"\n[TCP 서버] 클라이언트 접속 : IP 주소 = %s, 포트 번호 = %d\n", addr, ntohs(clientaddr.sin_port));

        ClientAddr* ca = new ClientAddr;
        ca->s = client_sock;
        ca->sockin = clientaddr;

        hThread = (HANDLE*)_beginthreadex(NULL, 0, processClient, (LPVOID)ca, 0, NULL);
        if (hThread == NULL)
        {
            closesocket(client_sock);
        }
        else
        {
            CloseHandle(hThread);
        }
    }

    closesocket(sock);
    WSACleanup();

    return 0;
}*/