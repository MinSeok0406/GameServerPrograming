#include <iostream>
#include <tchar.h>
#include <queue>
#include <stack>
#include <fcntl.h>
#include <io.h>
#include <conio.h>
#include <chrono>
#include <WinSock2.h>
using namespace std;

#pragma comment(lib, "Winmm.lib")

int a;
int b;

void Test2(int x)
{
    x++;
    _getch();
}

void Test()
{
    a++;
    b++;

    if (b == 1)
    {
        int x = 0;
        x++;
        Test2(x);
    }
}

int main()
{
    timeBeginPeriod(1);

    Test();

    return 0;
}