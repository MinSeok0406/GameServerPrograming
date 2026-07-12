#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <time.h>
#include <fcntl.h>
#include <io.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

#pragma pack(1)
struct STRUCT
{
    int a;
    char b;
    int c;
    char d;
};

#pragma pack()
STRUCT s = {1, 2, 3, 4};

int wmain(int argc, WCHAR* argv[])
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    timeBeginPeriod(1);

    
    
    

    return 0;
}
