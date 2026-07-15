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
    const wchar_t* testdata[] =
    {
        L"안녕하세요",
        L"반갑워요",
        L"오늘따라 할 이야기가 많을 것 같네요",
        L"저도 그렇네요"
    };

    for (auto i = 0; i < 4; ++i)
    {
        int len = static_cast<int>(BUFSIZE * sizeof(wchar_t));

        wmemset(buf, L'#', BUFSIZE);
        wcsncpy(buf, testdata[i], wcslen(testdata[i]));

        retval = send(sock, (char*)buf, len, 0);
        if (retval == SOCKET_ERROR)
        {
            break;
        }

        wprintf(L"[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);
    }

    closesocket(sock);
    WSACleanup();

    return 0;
}*/
