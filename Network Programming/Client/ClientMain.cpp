#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include "resource.h"
#include "PrjClient.h"
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

extern  HINSTANCE    g_hInstance;
extern  HWND         g_hBtnSendFile;
extern  HWND         g_hBtnSendMsg;
extern  HWND         g_hEditStatus;
extern  HWND         g_hBtnErasePic;
extern  HWND         g_hDrawWnd;

extern  volatile bool        g_isIPv6;
extern  wchar_t              g_ipaddr[64];
extern  int                  g_port;
extern  volatile bool        g_isUDP;
extern  HANDLE               g_hClientThread;
extern  volatile bool        g_bCommStarted;
extern  SOCKET               g_sock;
extern  HANDLE               g_hReadEvent;
extern  HANDLE               g_hWriteEvent;
extern  CHAT_MSG             g_chatmsg;
extern  DRAWLINE_MSG         g_drawlinemsg;
extern  int                  g_drawcolor;
extern  ERASEPIC_MSG         g_erasepicmsg;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    timeBeginPeriod(1);
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 1;
    }
    
    g_hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
    g_hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    g_chatmsg.type = TYPE_CHAT;
    g_drawlinemsg.type = TYPE_DRAWLINE;
    g_drawlinemsg.color = RGB(255, 0, 0);
    g_erasepicmsg.type = TYPE_ERASEPIC;

    g_hInstance = hInstance;
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);

    CloseHandle(g_hReadEvent);
    CloseHandle(g_hWriteEvent);

    WSACleanup();

    return 0;
}
