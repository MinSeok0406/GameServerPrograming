#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <tchar.h>
#include <process.h>
#include <Windows.h>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")

void err_quit(const WCHAR* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
    LocalFree(lpMsgBuf);
    exit(1);
}

int wmain(int argc, WCHAR* argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    timeBeginPeriod(1);

    WSAData wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 1;
    }

    MessageBox(NULL, L"Init Success", L"Notice", MB_OK);

    SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_sock == INVALID_SOCKET)
    {
        err_quit(L"socket()");
    }

    MessageBox(NULL, L"TCP Success", L"Notice", MB_OK);

    closesocket(tcp_sock);

    WSACleanup();
    return 0;
}
