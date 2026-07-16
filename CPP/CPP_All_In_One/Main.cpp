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

const int dy[4] = { 0, 1, 0, -1 };
const int dx[4] = { 1, 0, -1, 0 };
int n, m, k;
int cnt, ret;
int adj[104][104];
int visited[104][104];
vector<int> v;

void Dfs(int y, int x)
{
    visited[y][x] = 1;
    for (auto i = 0; i < 4; ++i)
    {
        int ny = y + dy[i];
        int nx = x + dx[i];

        if (ny < 0 || ny >= m || nx < 0 || nx >= n || adj[ny][nx] == 1)
        {
            continue;
        }

        if (visited[ny][nx] == 0)
        {
            ret++;
            Dfs(ny, nx);
        }
    }
}

int wmain(int argc, WCHAR* argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    timeBeginPeriod(1);

    cin >> m >> n >> k;

    for (auto i = 0; i < k; ++i)
    {
        int x1, x2, y1, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        for (auto y = y1; y < y2; ++y)
        {
            for (auto x = x1; x < x2; ++x)
            {
                adj[y][x] = 1;
            }
        }
    }

    for (auto i = 0; i < m; ++i)
    {
        for (auto j = 0; j < n; ++j)
        {
            if (visited[i][j] == 0 && adj[i][j] == 0)
            {
                ret = 1;
                Dfs(i, j);
                cnt++;
                v.push_back(ret);
            }
        }
    }

    sort(v.begin(), v.end());

    cout << cnt << "\n";
    for (auto i : v)
    {
        cout << i << " ";
    }

    return 0;
}
