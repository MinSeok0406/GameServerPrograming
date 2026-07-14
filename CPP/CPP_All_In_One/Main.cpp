#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <tchar.h>
#include <process.h>
#include <Windows.h>
#include <vector>
#include <map>
#include <string>
#include <stack>
#include <queue>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")

const int dy[4] = { 0, 1, 0, -1 };
const int dx[4] = { 1, 0, -1, 0 };
int t, n, m, k, cnt;
int adj[54][54];
int visited[54][54];

void Bfs(int y, int x)
{
    visited[y][x] = 1;
    queue<pair<int, int>> q;
    q.push({ y, x });

    while (q.empty() == false)
    {
        tie(y, x) = q.front();
        q.pop();

        for (auto i = 0; i < 4; ++i)
        {
            int ny = y + dy[i];
            int nx = x + dx[i];

            if (ny < 0 || ny >= n || nx < 0 || nx >= m || adj[ny][nx] == 0)
            {
                continue;
            }

            if (visited[ny][nx])
            {
                continue;
            }

            visited[ny][nx] = visited[y][x] + 1;
            q.push({ ny, nx });
        }
    }
}

int wmain(int argc, WCHAR* argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    timeBeginPeriod(1);

    cin >> t;

    while (t--)
    {
        cin >> m >> n >> k;

        cnt = 0;
        for (auto i = 0; i < 54; ++i)
        {
            for (auto j = 0; j < 54; ++j)
            {
                adj[i][j] = 0;
                visited[i][j] = 0;
            }
        }

        for (auto i = 0; i < k; ++i)
        {
            int x, y;
            cin >> x >> y;
            adj[y][x] = 1;
        }

        for (auto i = 0; i < n; ++i)
        {
            for (auto j = 0; j < m; ++j)
            {
                if (visited[i][j] == 0 && adj[i][j] == 1)
                {
                    Bfs(i, j);
                    cnt++;
                }
            }
        }

        cout << cnt << "\n";
    }

    return 0;
}
