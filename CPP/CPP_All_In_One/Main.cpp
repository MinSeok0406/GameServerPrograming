#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <tchar.h>
#include <process.h>
#include <Windows.h>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <stack>
#include <queue>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")

int n, c;
vector<pair<int, int>> v;
map<int, int> mp;
map<int, int> firstmp;

bool comp(pair<int, int> a, pair<int, int> b)
{
    if (a.second == b.second)
    {
        return firstmp[a.first] < firstmp[b.first];
    }

    return a.second > b.second;
}

int wmain(int argc, WCHAR* argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    timeBeginPeriod(1);

    cin >> n >> c;

    for (auto i = 0; i < n; ++i)
    {
        int k;
        cin >> k;

        if (firstmp.find(k) == firstmp.end())
        {
            firstmp[k] = i;
        }
        
        mp[k]++;
    }

    for (auto i : mp)
    {
        v.push_back({ i.first, i.second });
    }

    sort(v.begin(), v.end(), comp);

    for (auto i : v)
    {
        for (auto j = 0; j < i.second; ++j)
        {
            cout << i.first << " ";
        }
    }

    return 0;
}
