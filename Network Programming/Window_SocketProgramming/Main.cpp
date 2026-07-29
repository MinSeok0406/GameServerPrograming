#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <time.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <windowsx.h>
#include <string>
#include "RingBuffer.h"
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

#define SERVERPORT      25000
#define BUFSIZE         512
#define UM_NETWORK      (WM_USER+1)

struct stHEADER
{
    unsigned short _len;
};

#pragma pack(1)
struct st_DRAW_PACKET
{
    stHEADER _header;
    int _startX;
    int _startY;
    int _endX;
    int _endY;
};

void procRecv(HWND hWnd);
void procSend();
void sendPacket(st_DRAW_PACKET* packet, int size);
LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
void processSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

bool g_connect = false;
bool g_bClick = false;
HPEN g_hPen;
int g_iOldX;
int g_iOldY;

wchar_t g_serverIP[INET_ADDRSTRLEN];
SOCKET g_clientsock;
SOCKADDR_IN g_clientaddr;
RingBuffer sendQ{ BUFSIZE };
RingBuffer recvQ{ BUFSIZE };

int wmain(int argc, WCHAR* argv[])
{
    timeBeginPeriod(1);
    srand(unsigned int(time(nullptr)));

    WNDCLASS wndclass;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = wndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = NULL;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = L"MyWndClass";
    if (!RegisterClass(&wndclass))
    {
        return 1;
    }

    HWND hWnd = CreateWindow(L"MyWndClass", L"DrawLine", WS_OVERLAPPEDWINDOW, 0, 0, 1280, 720, NULL, NULL, NULL, NULL);
    if (hWnd == NULL)
    {
        return 1;
    }
    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow(hWnd);

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 1;
    }

    fgetws(g_serverIP, 100, stdin);
    int len = (int)wcslen(g_serverIP);
    g_serverIP[len - 1] = '\0';

    g_clientaddr.sin_family = AF_INET;
    g_clientaddr.sin_port = htons(SERVERPORT);
    InetPton(AF_INET, g_serverIP, &g_clientaddr.sin_addr);

    MSG msg;
    while (GetMessage(&msg, 0, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    WSACleanup();

    return (int)msg.wParam;
}

void procRecv(HWND hWnd)
{
    if (recvQ.GetFreeSize() < sizeof(st_DRAW_PACKET))
    {
        return;
    }

    int recvRet = recv(g_clientsock, recvQ.GetRearBufferPtr(), recvQ.DirectEnqueueSize(), 0);
    if (recvRet == SOCKET_ERROR)
    {
        if (WSAGetLastError() != EWOULDBLOCK)
        {
            printf("%d\n", WSAGetLastError());
            return;
        }
    }
    else if (recvRet == 0)
    {
        return;
    }

    recvQ.MoveRear(recvRet);

    while (true)
    {
        if (recvQ.GetUseSize() < sizeof(st_DRAW_PACKET))
        {
            break;
        }

        char buf[BUFSIZE];
        int peekRet = recvQ.Peek(buf, sizeof(st_DRAW_PACKET));

        // 멀티쓰레드를 위한 코드
        if (peekRet != sizeof(st_DRAW_PACKET))
        {
            __debugbreak();
        }

        recvQ.MoveRear(peekRet);

        stHEADER* header = (stHEADER*)buf;
        if (header->_len != 16)
        {
            __debugbreak();
        }

        int* bbuf = (int*)(buf + 2);

        int startX = buf[0];
        int startY = buf[1];
        int endX = buf[2];
        int endY = buf[3];

        HDC hdc = GetDC(hWnd);
        HPEN hPenOld = (HPEN)SelectObject(hdc, g_hPen);
        MoveToEx(hdc, startX, startY, NULL);
        LineTo(hdc, endX, endY);
        SelectObject(hdc, hPenOld);
        ReleaseDC(hWnd, hdc);
    }
}

void procSend()
{
    while (true)
    {
        if (sendQ.GetUseSize() < sizeof(st_DRAW_PACKET))
        {
            break;
        }

        int sendRet = send(g_clientsock, sendQ.GetFrontBufferPtr(), sendQ.DirectDequeueSize(), 0);
        if (sendRet == SOCKET_ERROR)
        {
            if (WSAGetLastError() != EWOULDBLOCK)
            {
                printf("%d\n", WSAGetLastError());
                break;
            }
        }
    }
}

void sendPacket(st_DRAW_PACKET* packet, int size)
{
    if (sendQ.GetFreeSize() < sizeof(st_DRAW_PACKET))
    {
        __debugbreak();
    }

    int sendQRet = sendQ.Enqueue((char*)packet, size);

    procSend();
}

LRESULT wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int AsyncRet = 0;
    int connectRet = 0;

    switch (uMsg)
    {
    case WM_CREATE:
        g_hPen = CreatePen(PS_SOLID, rand() % 20, RGB(rand() % 255, rand() % 255, rand() % 255));
        g_clientsock = socket(AF_INET, SOCK_STREAM, 0);
        if (g_clientsock == INVALID_SOCKET)
        {
            printf("%d\n", WSAGetLastError());
            break;
        }

        AsyncRet = WSAAsyncSelect(g_clientsock, hWnd, UM_NETWORK, FD_READ | FD_WRITE | FD_CONNECT | FD_CLOSE);
        if (AsyncRet == SOCKET_ERROR)
        {
            printf("%d\n", WSAGetLastError());
            break;
        }

        connectRet = connect(g_clientsock, (SOCKADDR*)&g_clientaddr, sizeof(g_clientaddr));
        if (connectRet == SOCKET_ERROR)
        {
            if (WSAGetLastError() != EWOULDBLOCK)
            {
                printf("%d\n", WSAGetLastError());
                break;
            }
        }
        break;
    case WM_RBUTTONDOWN:
        if (g_hPen != NULL)
        {
            DeleteObject(g_hPen);
            g_hPen = CreatePen(PS_SOLID, rand() % 20, RGB(rand() % 255, rand() % 255, rand() % 255));
        }
        break;
    case WM_LBUTTONDOWN:
        g_bClick = true;
        break;
    case WM_LBUTTONUP:
        g_bClick = false;
        break;
    case WM_MOUSEMOVE:
    {
        if (!g_connect)
        {
            break;
        }
        
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);
        if (g_bClick)
        {
            HDC hdc = GetDC(hWnd);
            HPEN hPenOld = (HPEN)SelectObject(hdc, g_hPen);
            MoveToEx(hdc, g_iOldX, g_iOldY, NULL);
            LineTo(hdc, xPos, yPos);
            SelectObject(hdc, hPenOld);
            ReleaseDC(hWnd, hdc);
        }

        st_DRAW_PACKET drawPacket{ 16, g_iOldX, g_iOldY, xPos, yPos };

        g_iOldX = xPos;
        g_iOldY = yPos;

        sendPacket(&drawPacket, sizeof(st_DRAW_PACKET));

        break;
    }
    case UM_NETWORK:
        processSocketMessage(hWnd, uMsg, wParam, lParam);
        break;
    case WM_DESTROY:
        DeleteObject(g_hPen);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

void processSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (WSAGETSELECTERROR(lParam))
    {
        closesocket(g_clientsock);
        return;
    }

    switch (WSAGETSELECTEVENT(lParam))
    {
    case FD_CONNECT:
        g_connect = true;
        break;
    case FD_CLOSE:
        closesocket(g_clientsock);
        break;
    case FD_READ:
        procRecv(hWnd);
        break;
    case FD_WRITE:
        procSend();
        break;
    }
}