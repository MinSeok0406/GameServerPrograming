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

static int total = 0;

unsigned int WINAPI ThreadProc(LPVOID lpParam)
{
    DWORD* nPtr = (DWORD*)lpParam;

    DWORD numOne = *nPtr;
    DWORD numTwo = *(nPtr + 1);

    for (DWORD i = numOne; i <= numTwo; ++i)
    {
        total += i;
    }

    return total;
}

int _tmain(int argc, TCHAR* argv[])
{
    timeBeginPeriod(1);
    system("chcp 65001");
    
    DWORD dwThreadID[3];
    HANDLE hThread[3];

    DWORD paramThread[] = { 1, 3, 4, 7, 8, 10 };

    for (auto i = 0; i < 3; i++)
    {
        hThread[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, (LPVOID)(&paramThread[i * 2]), 0, (unsigned*)&dwThreadID[i]);

        if (hThread[i] == NULL)
        {
            tcout << "Thread creation fault!" << "\n";
            return -1;
        }
    }

    WaitForMultipleObjects(3, hThread, TRUE, INFINITE);

    tcout << "total (1 ~ 10): " << total << "\n";

    for (auto i = 0; i < 3; i++)
    {
        CloseHandle(hThread[i]);
    }
    
    return 0;
}
