/*#include <iostream>
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

const int dy[4] = { 1, 0, -1, 0 };
const int dx[4] = { 0, 1, 0, -1 };

#define HEIGHT 10
#define WIDTH 20

char adj[HEIGHT][WIDTH] = {
    {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'},
    {'x', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'x'},
    {'x', ' ', ' ', 'x', ' ', 'x', ' ', 'x', ' ', ' ', ' ', ' ', ' ', ' ', 'x', ' ', ' ', ' ', ' ', 'x'},
    {'x', ' ', 'x', 'x', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'x'},
    {'x', ' ', ' ', 'x', ' ', 'x', ' ', ' ', ' ', ' ', 'x', ' ', ' ', 'x', ' ', ' ', 'x', ' ', ' ', 'x'},
    {'x', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'x', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'x'},
    {'x', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'x', ' ', ' ', ' ', 'x', ' ', 'x'},
    {'x', ' ', ' ', ' ', ' ', 'x', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'x'},
    {'x', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', ' '}
};

char cpyadj[HEIGHT][WIDTH];
int visited[HEIGHT][WIDTH];
pair<int, int> parentAdj[HEIGHT][WIDTH];
bool success = false;

void Print()
{
    cout << "DFS Search" << "\n";
    //cout << "BFS Search" << "\n";

    for (auto i = 0; i < HEIGHT; i++)
    {
        for (auto j = 0; j < WIDTH; j++)
        {
            cout << cpyadj[i][j];
        }
        cout << "\n";
    }
}

void PrintStar(int sy, int sx)
{
    if (sy == -1 && sx == -1)
    {
        return;
    }

    adj[sy][sx] = '*';

    int ny = parentAdj[sy][sx].first;
    int nx = parentAdj[sy][sx].second;
    PrintStar(ny, nx);

    return;
}

void Dfs(int sy, int sx, int ey, int ex)
{
    if (sy == ey && sx == ex)
    {
        success = true;
        return;
    }

    visited[sy][sx] = 1;
    for (auto k = 0; k < 4; k++)
    {
        int ny = sy + dy[k];
        int nx = sx + dx[k];

        if (ny < 0 || ny >= HEIGHT || nx < 0 || nx >= WIDTH || cpyadj[ny][nx] == 'x')
        {
            continue;
        }

        if (visited[ny][nx] == 0)
        {
            cpyadj[ny][nx] = '.';
            parentAdj[ny][nx] = { sy, sx };
            visited[ny][nx] = 1;

            system("cls");
            Print();

            Dfs(ny, nx, ey, ex);
        }

        if (success)
        {
            return;
        }
    }

    return;
}

void Bfs(int sy, int sx, int ey, int ex)
{
    visited[sy][sx] = 1;
    queue<pair<int, int>> q;
    q.push({ sy, sx });

    while (q.empty() == false)
    {
        int y = q.front().first;
        int x = q.front().second;
        q.pop();

        for (auto k = 0; k < 4; k++)
        {
            int ny = y + dy[k];
            int nx = x + dx[k];

            if (ny < 0 || ny >= HEIGHT || nx < 0 || nx >= WIDTH || cpyadj[ny][nx] == 'x')
            {
                continue;
            }

            if (visited[ny][nx] == 0)
            {
                cpyadj[ny][nx] = '.';
                parentAdj[ny][nx] = { y, x };

                system("cls");
                Print();

                visited[ny][nx] = 1;
                q.push({ ny, nx });
            }
        }
    }
}

int main()
{
    timeBeginPeriod(1);

    cout << "\n";
    for (auto i = 0; i < HEIGHT; i++)
    {
        for (auto j = 0; j < WIDTH; j++)
        {
            cout << adj[i][j];
        }
        cout << "\n";
    }

    Sleep(3000);
    memcpy_s(cpyadj, sizeof(cpyadj), adj, sizeof(adj));
    memset(visited, 0, sizeof(visited));
    parentAdj[1][1] = { -1, -1 };
    cpyadj[1][1] = '.';
    success = false;

    Dfs(1, 1, 9, 19);
    //Bfs(1, 1, 9, 19);

    PrintStar(9, 19);
    system("cls");

    for (auto i = 0; i < HEIGHT; i++)
    {
        for (auto j = 0; j < WIDTH; j++)
        {
            cout << adj[i][j];
        }
        cout << "\n";
    }

    return 0;
}*/