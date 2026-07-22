#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
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
#define WM_SOCKET   (WM_USER+1)

struct SOCKETINFO
{
    SOCKET sock;
    char buf[BUFSIZE + 1];
    int recvbytes;
    int sendbytes;
    bool recvdelayed;
    SOCKETINFO* next;
};

SOCKETINFO* SocketInfoList;

LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
void processSocketMessage(HWND, UINT, WPARAM, LPARAM);

bool AddSocketInfo(SOCKET sock);
SOCKETINFO* GetSocketInfo(SOCKET sock);
void RemoveSocketInfo(SOCKET sock);

int wmain(int argc, WCHAR* argv[])
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    timeBeginPeriod(1);
    srand(unsigned int(time(nullptr)));

    int retval;

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

    HWND hWnd = CreateWindow(L"MyWndClass", L"TCP 서버", WS_OVERLAPPEDWINDOW, 0, 0, 600, 200, NULL, NULL, NULL, NULL);
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
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR)
    {
        wprintf(L"%d\n", WSAGetLastError());
        return 1;
    }
    
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR)
    {
        wprintf(L"%d\n", WSAGetLastError());
        return 1;
    }

    retval = WSAAsyncSelect(listen_sock, hWnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE);
    if (retval == SOCKET_ERROR)
    {
        wprintf(L"%d\n", WSAGetLastError());
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, 0, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    WSACleanup();

    return msg.wParam;
}

LRESULT wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_SOCKET:
        processSocketMessage(hWnd, uMsg, wParam, lParam);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void processSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int retval;
    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;
    SOCKETINFO* ptr;

    if (WSAGETSELECTERROR(lParam))
    {
        RemoveSocketInfo(wParam);
        return;
    }

    switch (WSAGETSELECTEVENT(lParam))
    {
    case FD_ACCEPT:
        addrlen = sizeof(clientaddr);
        client_sock = accept(wParam, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET)
        {
            wprintf(L"%d\n", WSAGetLastError());
            return;
        }

        wprintf(L"\n[TCP 서버] 클라이언트 접속 : IP 주소=%hs, 포트 번호=%d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        AddSocketInfo(client_sock);
        retval = WSAAsyncSelect(client_sock, hWnd, WM_SOCKET, FD_READ | FD_WRITE | FD_CLOSE);
        if (retval == SOCKET_ERROR)
        {
            wprintf(L"%d\n", WSAGetLastError());
            RemoveSocketInfo(client_sock);
        }
        break;
    case FD_READ:
        ptr = GetSocketInfo(wParam);
        if (ptr->recvbytes > 0)
        {
            ptr->recvdelayed = true;
            return;
        }

        retval = recv(ptr->sock, ptr->buf, BUFSIZE, 0);
        if (retval == SOCKET_ERROR)
        {
            wprintf(L"%d\n", WSAGetLastError());
            RemoveSocketInfo(wParam);
            return;
        }
        ptr->recvbytes = retval;
        ptr->buf[retval] = '\0';
        addrlen = sizeof(clientaddr);
        getpeername(wParam, (SOCKADDR*)&clientaddr, &addrlen);
        wprintf(L"\n[TCP/%hs:%d] %hs\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), ptr->buf);
    case FD_WRITE:
        ptr = GetSocketInfo(wParam);
        if (ptr->recvbytes <= ptr->sendbytes)
        {
            return;
        }

        retval = send(ptr->sock, ptr->buf + ptr->sendbytes, ptr->recvbytes - ptr->sendbytes, 0);
        if (retval == SOCKET_ERROR)
        {
            wprintf(L"%d\n", WSAGetLastError());
            RemoveSocketInfo(wParam);
            return;
        }

        ptr->sendbytes += retval;
        if (ptr->recvbytes == ptr->sendbytes)
        {
            ptr->recvbytes = ptr->sendbytes = 0;
            if (ptr->recvdelayed)
            {
                ptr->recvdelayed = false;
                PostMessage(hWnd, WM_SOCKET, wParam, FD_READ);
            }
        }
        break;
    case FD_CLOSE:
        RemoveSocketInfo(wParam);
        break;
    }
}

bool AddSocketInfo(SOCKET sock)
{
    SOCKETINFO* ptr = new SOCKETINFO;
    if (ptr == NULL)
    {
        wprintf(L"[오류] 메모리가 부족합니다.\n");
        return false;
    }

    ptr->sock = sock;
    ptr->recvbytes = 0;
    ptr->sendbytes = 0;
    ptr->recvdelayed = false;
    ptr->next = SocketInfoList;
    SocketInfoList = ptr;

    return true;
}

SOCKETINFO* GetSocketInfo(SOCKET sock)
{
    SOCKETINFO* ptr = SocketInfoList;
    while (ptr)
    {
        if (ptr->sock == sock)
        {
            return ptr;
        }
        ptr = ptr->next;
    }

    return nullptr;
}

void RemoveSocketInfo(SOCKET sock)
{
    SOCKADDR_IN clientaddr;
    int addrlen = sizeof(clientaddr);
    getpeername(sock, (SOCKADDR*)&clientaddr, &addrlen);
    wprintf(L"\n[TCP 서버] 클라이언트 종료 : IP 주소=%hs, 포트 번호=%d\n",
        inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

    SOCKETINFO* curr = SocketInfoList;
    SOCKETINFO* prev = NULL;
    while (curr)
    {
        if (curr->sock == sock)
        {
            if (prev)
            {
                prev->next = curr->next;
            }
            else
            {
                SocketInfoList = curr->next;
            }
            closesocket(curr->sock);
            delete curr;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}