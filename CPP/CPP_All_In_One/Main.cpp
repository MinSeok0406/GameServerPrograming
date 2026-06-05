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

class Widget
{
public:
    Widget() { cout << "Widget" << "\n"; }
    ~Widget() { cout << "~Widget" << "\n"; }
}; 

unsigned int WINAPI threadProc(LPVOID lpParam)
{
    unique_ptr<Widget> uptr = make_unique<Widget>();


    return 0;
}

int _tmain(int argc, TCHAR* argv[])
{
    timeBeginPeriod(1);
    system("chcp 65001");

    HANDLE hThread;
    DWORD dwThread;

    hThread = (HANDLE)_beginthreadex(NULL, 0, threadProc, NULL, 0, (unsigned*)&dwThread);

    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(hThread);

    

    return 0;
}
