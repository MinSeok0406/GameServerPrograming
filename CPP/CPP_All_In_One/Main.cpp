#include <iostream>
#include <tchar.h>
#include <process.h>
#include <Windows.h>
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

TCHAR str[100];
HANDLE hEvent;
HANDLE hMutex;

unsigned int WINAPI outputThreadFunction(LPVOID lpParam)
{
    WaitForSingleObject(hEvent, INFINITE);
    WaitForSingleObject(hMutex, INFINITE);

    tcout << "Output string : " << str << "\n";

    ReleaseMutex(hMutex);

    return 0;
}

unsigned int WINAPI countThreadFunction(LPVOID lpParam)
{
    WaitForSingleObject(hEvent, INFINITE);
    WaitForSingleObject(hMutex, INFINITE);

    tcout << "output string length : " << _tcslen(str) - 1 << "\n";

    ReleaseMutex(hMutex);
    return 0;
}

int _tmain(int argc, TCHAR* argv[])
{
    timeBeginPeriod(1);
    system("chcp 65001");
    
    HANDLE hThread[2];
    DWORD dwThreadID[2];

    hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    hMutex = CreateMutex(NULL, FALSE, NULL);

    hThread[0] = (HANDLE)_beginthreadex(NULL, 0, outputThreadFunction, NULL, 0, (unsigned*)&dwThreadID[0]);
    hThread[1] = (HANDLE)_beginthreadex(NULL, 0, countThreadFunction, NULL, 0, (unsigned*)&dwThreadID[1]);

    _fputts(_T("Insert string : "), stdout);
    _fgetts(str, 30, stdin);

    SetEvent(hEvent);

    WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
    CloseHandle(hThread[0]);
    CloseHandle(hThread[1]);
    CloseHandle(hEvent);
    CloseHandle(hMutex);
    
    return 0;
}
