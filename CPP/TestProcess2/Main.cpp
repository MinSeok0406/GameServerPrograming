#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <tchar.h>
#include <queue>
#include <stack>
#include <fcntl.h>
#include <io.h>
#include <conio.h>
#include <chrono>
#include <strsafe.h>
#include <WinSock2.h>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")

#ifdef UNICODE
#define tcout std::wcout
#define tcin std::wcin
#define tcerr std::wcerr
#define tclog std::wclog
#else
#define tcout std::cout
#define tcin std::cin
#define tcerr std::cerr
#define tclog std::clog
#endif

#ifdef _UNICODE
#define tcout std::wcout
#define tcin std::wcin
#define tcerr std::wcerr
#define tclog std::wclog
#else
#define tcout std::cout
#define tcin std::cin
#define tcerr std::cerr
#define tclog std::clog
#endif

#define SLOT_NAME   _T("\\\\.\\mailslot\\mailbox")

int _tmain(int argc, TCHAR* argv[])
{
    timeBeginPeriod(1);
    system("chcp 65001");

    HANDLE hMailSlot;
    TCHAR message[50];
    DWORD bytesWritten;

    FILE* file;
    _tfopen_s(&file, _T("InheritableHandle.txt"), _T("rt"));
    _ftscanf(file, _T("%d"), &hMailSlot);
    fclose(file);
    _tprintf(_T("Inheritable Handle : %d \n"), hMailSlot);

    while (true)
    {
        _fputts(_T("MY CMD>"), stdout);
        _fgetts(message, sizeof(message) / sizeof(TCHAR), stdin);

        if (!WriteFile(hMailSlot, message, _tcslen(message) * sizeof(TCHAR), &bytesWritten, NULL))
        {
            _fputts(_T("Unable to Write!"), stdout);
            CloseHandle(hMailSlot);
            return 1;
        }

        if (!_tcscmp(message, _T("exit")))
        {
            _fputts(_T("Good Bye"), stdout);
            break;
        }
    }

    CloseHandle(hMailSlot);
    return 0;
}