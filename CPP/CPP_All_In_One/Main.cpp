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

class Base
{
public:

    int Test()
    {
        int a = 2;

        int ret = Test2();

        return ret;
    }

private:
    virtual int Test2()
    {


        return 0;
    }
};

class Derived : public Base
{
public:
    int Test2() override
    {
        int b = 4;

        b++;
        b++;

        return b;
    }
};

int wmain(int argc, WCHAR* argv[])
{
    timeBeginPeriod(1);
    system("chcp 65001");

    Base* b = new Derived;

    b->Test();

    return 0;
}
