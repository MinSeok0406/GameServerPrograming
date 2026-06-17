#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <tchar.h>
#include <process.h>
#include <Windows.h>
#include <vector>
#include <list>
#include <algorithm>
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



int wmain(int argc, WCHAR* argv[])
{
    timeBeginPeriod(1);
    system("chcp 65001");

    
    

    return 0;
}
