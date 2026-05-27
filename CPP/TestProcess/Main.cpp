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

    hMailSlot = CreateFile(SLOT_NAME, GENERIC_WRITE, FILE_SHARE_READ,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

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