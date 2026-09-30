#pragma once

#define WIN32_LEAN_AND_MEAN
#include <process.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>

#define SERVERPORT 47000

#define SIZE_TOT 256
#define SIZE_DAT (SIZE_TOT - sizeof(int))

#define TYPE_CHAT 1000
#define TYPE_DRAWLINE 1001
#define TYPE_ERASEPIC 1002

#define WM_DRAWLINE (WM_USER + 1)
#define WM_ERASEPIC (WM_USER + 2)

struct COMM_MSG
{
    int type;
    char dummy[SIZE_DAT];
};

struct CHAT_MSG
{
    int type;
    char msg[SIZE_DAT];
};

struct DRAWLINE_MSG
{
    int type;
    int color;
    int sy;
    int sx;
    int ey;
    int ex;
    char dummy[SIZE_TOT - 6 * sizeof(int)];
};

struct ERASEPIC_MSG
{
    int type;
    char dummy[SIZE_DAT];
};

long long CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
long long CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);

unsigned int WINAPI ClientMain(LPVOID arg);
unsigned int WINAPI ReadThread(LPVOID arg);
unsigned int WINAPI WriteThread(LPVOID arg);

void DisplayText(const char* fmt, ...);