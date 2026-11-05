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

uint64_t g_count = 1000;

SOCKET sock;
const char* buf[4] = { "Hello", "MyName", "IsMinseok", "Bye~" };

bool update();
bool sendAll(const char* data, int len);
bool recvAll(char* data, int len);

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
        printf("connect : %d\n", WSAGetLastError());
        return 1;
    }

    uint64_t success = 0;
    while (g_count--)
    {
        if (!update())
        {
            break;
        }
        ++success;
    }

    printf("done : %llu messages verified\n", success);

    getchar();

    closesocket(sock);
    WSACleanup();

    return 0;
}

bool update()
{
    int index = rand() % 4;
    const char* sbuf = buf[index];
    int len = (int)strlen(sbuf);

    if (len >= BUFSIZE)
    {
        printf("message too long : %d\n", len);
        return false;
    }

    if (!sendAll(sbuf, len))
    {
        return false;
    }

    // 보낸 만큼 다 받을 때까지 기다린다.
    // 서버가 에코를 여러 조각으로 나눠 보내도 여기서 다시 맞춰진다.
    char rbuf[BUFSIZE];
    if (!recvAll(rbuf, len))
    {
        return false;
    }

    rbuf[len] = '\0';
    cout << rbuf << "\n";

    // 서버 검증: 보낸 것과 받은 것이 다르면 바로 멈춘다
    if (memcmp(sbuf, rbuf, len) != 0)
    {
        printf("MISMATCH! sent [%s] recv [%s]\n", sbuf, rbuf);
        return false;
    }

    return true;
}

// len 바이트를 모두 보낼 때까지 반복
bool sendAll(const char* data, int len)
{
    int sent = 0;
    while (sent < len)
    {
        int ret = send(sock, data + sent, len - sent, 0);
        if (ret == SOCKET_ERROR)
        {
            printf("send : %d\n", WSAGetLastError());
            return false;
        }
        sent += ret;
    }
    return true;
}

// len 바이트를 모두 받을 때까지 반복
bool recvAll(char* data, int len)
{
    int received = 0;
    while (received < len)
    {
        int ret = recv(sock, data + received, len - received, 0);
        if (ret == SOCKET_ERROR)
        {
            printf("recv : %d\n", WSAGetLastError());
            return false;
        }
        if (ret == 0)
        {
            printf("server closed\n");
            return false;
        }
        received += ret;
    }
    return true;
}
