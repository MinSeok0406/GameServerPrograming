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
#include <chrono>
#include "Temp.h"
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
    TCHAR messageBox[50];
    DWORD bytesRead;

    hMailSlot = CreateMailslot(SLOT_NAME, 0, MAILSLOT_WAIT_FOREVER, NULL);

    _fputts(_T("Message!!\n"), stdout);
    while (true)
    {
        if (!ReadFile(hMailSlot, messageBox, sizeof(TCHAR) * 50, &bytesRead, NULL))
        {
            _fputts(_T("Unable to read!"), stdout);
            CloseHandle(hMailSlot);
            return 1;
        }

        if (!_tcsncmp(messageBox, _T("exit"), 4))
        {
            _fputts(_T("Good Bye"), stdout);
            break;
        }

        messageBox[bytesRead / sizeof(TCHAR)] = 0;
        _fputts(messageBox, stdout);
    }

    CloseHandle(hMailSlot);
    return 0;
}