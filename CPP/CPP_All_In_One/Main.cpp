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

#define MAX_PAGE 10

int* nextPageAddr;
DWORD pageCnt;
DWORD pageSize;

int pageFaultExceptionFilter(DWORD exptCode)
{
    if (exptCode != EXCEPTION_ACCESS_VIOLATION)
    {
        return EXCEPTION_EXECUTE_HANDLER;
    }

    tcout << "Exception is a page fault" << "\n";

    if (pageCnt >= MAX_PAGE)
    {
        tcout << "Exception : out of pages" << "\n";
        return EXCEPTION_EXECUTE_HANDLER;
    }

    LPVOID lpvResult = VirtualAlloc((LPVOID)nextPageAddr, pageSize, MEM_COMMIT, PAGE_READWRITE);
    if (lpvResult == nullptr)
    {
        return EXCEPTION_EXECUTE_HANDLER;
    }
    else
    {
        tcout << "Allocating another page" << "\n";
    }

    pageCnt++;
    nextPageAddr += pageSize / sizeof(int);
    return EXCEPTION_CONTINUE_EXECUTION;
}

int _tmain(int argc, TCHAR* argv[])
{
    timeBeginPeriod(1);
    system("chcp 65001");

    LPVOID baseAddr;
    int* lpPtr;
    SYSTEM_INFO sSysInfo;

    GetSystemInfo(&sSysInfo);
    pageSize = sSysInfo.dwPageSize;

    baseAddr = VirtualAlloc(NULL, MAX_PAGE * pageSize, MEM_RESERVE, PAGE_NOACCESS);

    lpPtr = (int*)baseAddr;
    nextPageAddr = (int*)baseAddr;

    for (auto i = 0; i < (MAX_PAGE * pageSize) / sizeof(int); ++i)
    {
        __try
        {
            lpPtr[i] = i;
        }
        __except (pageFaultExceptionFilter(GetExceptionCode()))
        {
            ExitProcess(GetLastError());
        }
    }
    
    for (auto i = 0; i < (MAX_PAGE * pageSize) / sizeof(int); ++i)
    {
        tcout << lpPtr[i] << " ";
    }

    BOOL isSuccess = VirtualFree(baseAddr, 0, MEM_RESERVE);
    if (isSuccess)
    {
        tcout << "succeeded!" << "\n";
    }
    else
    {
        tcout << "failed" << "\n";
    }

    return 0;
}
