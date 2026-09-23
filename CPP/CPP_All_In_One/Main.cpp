#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <process.h>
#include <Windows.h>
#include <time.h>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")


int wmain()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    timeBeginPeriod(1);
    srand((unsigned int)time(nullptr));

    short x = 0x1234;
    long y = 0x12345678;

    short hx = ntohs(x);
    long hy = ntohl(y);

    short nx = htons(x);
    long ny = htonl(y);

    return 0;
}