#include <iostream>
#include <tchar.h>
#include <queue>
#include <stack>
#include <fcntl.h>
#include <io.h>
#include <conio.h>
#include <chrono>
#include <strsafe.h>
#include <WinSock2.h>
using namespace std;

#pragma comment(lib, "Winmm.lib")

extern "C" const IMAGE_DOS_HEADER __ImageBase;

using ll = long long;

string s;
ll g_maxValue = -1e9;
ll g_minValue = 1e9;

int _tmain(int argc, TCHAR* argv[])
{
    timeBeginPeriod(1);

    s.push_back('5');
    s.push_back('2');
    s.push_back('3');
    s.push_back('1');
    s.push_back('-1');
    s.push_back('-4');
    s.push_back('-7');

    ll value = 0;
    for (auto i = 0; i < s.size(); ++i)
    {
        if (s[i] == '-')
        {
            i++;
            value = static_cast<ll>(s[i] - '0');
            value *= -1;
        }
        else
        {
            value = static_cast<ll>(s[i] - '0');
        }

        g_maxValue = max(value, g_maxValue);
        g_minValue = min(value, g_minValue);
    }
    

    return 0;
}