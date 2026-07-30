#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <time.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <windowsx.h>
#include <list>
#include "RingBuffer.h"
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

#define SERVERPORT      25000
#define BUFSIZE         512
#define UM_NETWORK      (WM_USER+1)
#define TIMER_ID        1
#define TIMER_INTERVAL  1   // ms. 1초에 하나씩 긋고 전송
#define HEIGHT          720
#define WEIGHT          1280

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
void gdi_Drawing(HWND hWnd, int startX, int startY, int endX, int endY);
void generateAndSendLine(HWND hWnd);

bool g_connect = false;
bool g_bClick = false;
HPEN g_hPen;
int g_iOldX;
int g_iOldY;

wchar_t g_serverIP[INET_ADDRSTRLEN];
SOCKET g_clientsock;
SOCKADDR_IN g_clientaddr;
RingBuffer sendQ { BUFSIZE };
RingBuffer recvQ { BUFSIZE };

int wmain(int argc, WCHAR* argv[])
{
    timeBeginPeriod(1);
    srand(unsigned int(time(nullptr)));

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 1;
    }

    fgetws(g_serverIP, 100, stdin);
    int len = (int)wcslen(g_serverIP);
    g_serverIP[len - 1] = L'\0';

    memset(&g_clientaddr, 0, sizeof(g_clientaddr));
    g_clientaddr.sin_family = AF_INET;
    g_clientaddr.sin_port = htons(SERVERPORT);
    InetPton(AF_INET, g_serverIP, &g_clientaddr.sin_addr);

    WNDCLASS wndclass;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = wndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = GetModuleHandle(NULL);
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = L"MyWndClass";
    if (!RegisterClass(&wndclass))
    {
        return 1;
    }

    HWND hWnd = CreateWindow(L"MyWndClass", L"DrawLine", WS_OVERLAPPEDWINDOW, 0, 0, WEIGHT, HEIGHT, NULL, NULL, NULL, NULL);
    if (hWnd == NULL)
    {
        return 1;
    }
    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow(hWnd);

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
        if (WSAGetLastError() != WSAEWOULDBLOCK)
        {
            printf("%d\n", WSAGetLastError());
        }
        return;
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

        char buf[22];
        int peekRet = recvQ.Peek(buf, sizeof(st_DRAW_PACKET));

        // 멀티쓰레드를 위한 코드
        if (peekRet != sizeof(st_DRAW_PACKET))
        {
            __debugbreak();
        }

        recvQ.MoveFront(peekRet);

        stHEADER* header = (stHEADER*)buf;
        if (header->_len != 16)
        {
            __debugbreak();
        }

        int* bbuf = (int*)(buf + 2);

        int startX = bbuf[0];
        int startY = bbuf[1];
        int endX = bbuf[2];
        int endY = bbuf[3];

        gdi_Drawing(hWnd, startX, startY, endX, endY);
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
            if (WSAGetLastError() != WSAEWOULDBLOCK)
            {
                printf("%d\n", WSAGetLastError());
            }
            break;
        }

        sendQ.MoveFront(sendRet);
    }
}

void sendPacket(st_DRAW_PACKET* packet, int size)
{
    if (sendQ.GetFreeSize() > sizeof(st_DRAW_PACKET))
    {
        int sendQRet = sendQ.Enqueue((char*)packet, size);
        if (sendQRet == 0)
        {
            __debugbreak();
        }
    }

    procSend();
}

void generateAndSendLine(HWND hWnd)
{
    if (!g_connect)
    {
        return;
    }

    int xPos = rand() % WEIGHT;
    int yPos = rand() % HEIGHT;
    gdi_Drawing(hWnd, g_iOldX, g_iOldY, xPos, yPos);

    st_DRAW_PACKET drawPacket { 16, g_iOldX, g_iOldY, xPos, yPos };
    sendPacket(&drawPacket, sizeof(st_DRAW_PACKET));

    g_iOldX = xPos;
    g_iOldY = yPos;
}

LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
            if (WSAGetLastError() != WSAEWOULDBLOCK)
            {
                printf("%d\n", WSAGetLastError());
                return -1;
            }
        }

        AsyncRet = WSAAsyncSelect(g_clientsock, hWnd, UM_NETWORK, FD_READ | FD_WRITE | FD_CONNECT | FD_CLOSE);
        if (AsyncRet == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSAEWOULDBLOCK)
            {
                printf("%d\n", WSAGetLastError());
                return -1;
            }
        }

        connectRet = connect(g_clientsock, (SOCKADDR*)&g_clientaddr, sizeof(g_clientaddr));
        if (connectRet == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSAEWOULDBLOCK)
            {
                printf("%d\n", WSAGetLastError());
                return -1;
            }
        }

        SetTimer(hWnd, TIMER_ID, TIMER_INTERVAL, NULL);
        break;
    case WM_TIMER:
        if (wParam == TIMER_ID)
        {
            generateAndSendLine(hWnd);
        }
        break;
    case UM_NETWORK:
        processSocketMessage(hWnd, uMsg, wParam, lParam);
        break;
    case WM_DESTROY:
        KillTimer(hWnd, TIMER_ID);
        DeleteObject(g_hPen);
        closesocket(g_clientsock);
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

void gdi_Drawing(HWND hWnd, int startX, int startY, int endX, int endY)
{
    HDC hdc = GetDC(hWnd);
    HPEN hPenOld = (HPEN)SelectObject(hdc, g_hPen);
    MoveToEx(hdc, startX, startY, NULL);
    LineTo(hdc, endX, endY);
    SelectObject(hdc, hPenOld);
    ReleaseDC(hWnd, hdc);
}