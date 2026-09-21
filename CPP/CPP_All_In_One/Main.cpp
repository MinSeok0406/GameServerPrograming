#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <process.h>
#include <Windows.h>
#include <time.h>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")

void Test(int a, int b)
{
    int c = a + b;
    c++;
}

int wmain()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    timeBeginPeriod(1);
    srand((unsigned int)time(nullptr));

    Test(1, 2);

    return 0;
}