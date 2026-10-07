#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <process.h>
#include <Windows.h>
#include <winerror.h>
#include <time.h>
#include <vector>
#include <array>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")

constexpr int pow(int base, int exp) noexcept
{
    int result = 1;
    for (int i = 0; i < exp; ++i)
    {
        result *= base;
    }
    return result;
}

int wmain()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    timeBeginPeriod(1);
    srand((unsigned int)time(nullptr));

    array<int, pow(10, 3)> arr;
    
    int a = 5;

    return 0;
}