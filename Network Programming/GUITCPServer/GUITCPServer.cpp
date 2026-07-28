// GUITCPServer.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "GUITCPServer.h"
#include <iostream>
using namespace std;

#pragma comment(lib, "Ws2_32.lib")

#define MAX_LOADSTRING  100
#define SERVERPORT      47000
#define BUFSIZE         512

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND hEdit;
CRITICAL_SECTION cs;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

void DisplayText(const char* fmt, ...);
u_int WINAPI serverMain(LPVOID arg);
u_int WINAPI processClient(LPVOID arg);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    srand(unsigned int(time(nullptr)));
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    hInst = hInstance;
    InitializeCriticalSection(&cs);

    WNDCLASS wndclass;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = L"MyWndClass";
    if (!RegisterClass(&wndclass))
    {
        return 1;
    }

    HWND hWnd = CreateWindow(L"MyWndClass", L"TCP 서버", WS_OVERLAPPEDWINDOW, 0, 0, 500, 200,
        NULL, NULL, hInstance, NULL);
    if (hWnd == NULL)
    {
        return 1;
    }
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    _beginthreadex(NULL, 0, serverMain, NULL, 0, NULL);

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DeleteCriticalSection(&cs);
    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        hEdit = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_HSCROLL |
            WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_READONLY,
            0, 0, 0, 0, hWnd, (HMENU)100, hInst, NULL);
        return 0;
    case WM_SIZE:
        MoveWindow(hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
        return 0;
    case WM_SETFOCUS:
        SetFocus(hEdit);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void DisplayText(const char* fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    char cbuf[BUFSIZE * 2];
    vsprintf(cbuf, fmt, arg);
    va_end(arg);

    EnterCriticalSection(&cs);
    int nLength = GetWindowTextLength(hEdit);
    SendMessage(hEdit, EM_SETSEL, nLength, nLength);
    SendMessage(hEdit, EM_REPLACESEL, FALSE, (LPARAM)cbuf);
    LeaveCriticalSection(&cs);
}

u_int __stdcall serverMain(LPVOID arg)
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("%d\n", WSAGetLastError());
        return 1;
    }

    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET)
    {
        printf("%d\n", WSAGetLastError());
        return 1;
    }

    SOCKADDR_IN serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERVERPORT);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int bindRet = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (bindRet == SOCKET_ERROR)
    {
        printf("%d\n", WSAGetLastError());
        return 1;
    }

    int listenRet = listen(listen_sock, SOMAXCONN);
    if (listenRet == SOCKET_ERROR)
    {
        printf("%d\n", WSAGetLastError());
        return 1;
    }

    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;
    HANDLE hThread;

    while (true)
    {
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET)
        {
            printf("%d\n", WSAGetLastError());
            break;
        }

        wchar_t addr[INET_ADDRSTRLEN];
        InetNtop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
        DisplayText("\r\n[TCP 서버] 클라이언트 접속 : IP 주소 = %s, 포트 번호=%d\r\n", addr, ntohs(clientaddr.sin_port));

        hThread = (HANDLE)_beginthreadex(NULL, 0, processClient, (LPVOID)client_sock, 0, NULL);
        if (hThread == NULL)
        {
            closesocket(client_sock);
        }
        else
        {
            CloseHandle(hThread);
        }
    }

    closesocket(listen_sock);
    WSACleanup();
    return 0;
}

u_int __stdcall processClient(LPVOID arg)
{
    SOCKET client_sock = (SOCKET)arg;
    SOCKADDR_IN clientaddr;
    wchar_t addr[INET_ADDRSTRLEN];
    int addrlen;
    char buf[BUFSIZE + 1];

    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);
    InetNtop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

    while (true)
    {
        int recvRet = recv(client_sock, buf, BUFSIZE, 0);
        if (recvRet == SOCKET_ERROR)
        {
            printf("%d\n", WSAGetLastError());
            break;
        }
        else if (recvRet == 0)
        {
            break;
        }

        buf[recvRet] = '\0';
        DisplayText("[TCP/%s:%d] %s\r\n", addr, ntohs(clientaddr.sin_port), buf);

        int sendRet = send(client_sock, buf, recvRet, 0);
        if (sendRet == SOCKET_ERROR)
        {
            printf("%d\n", WSAGetLastError());
            break;
        }
    }

    closesocket(client_sock);
    DisplayText("\r\n[TCP 서버] 클라이언트 종료 : IP 주소 = %s, 포트 번호=%d\r\n", addr, ntohs(clientaddr.sin_port));
    return 0;
}
