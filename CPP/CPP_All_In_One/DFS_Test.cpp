/*#include <iostream>
#include <queue>
#include <stack>
#include <WinSock2.h>
using namespace std;

#define SCREEN_HEIGHT 10
#define SCREEN_WIDTH 20

struct stData
{
	int y;
	int x;
};

const int dy[4] = { 0, 1, 0, -1 };
const int dx[4] = { 1, 0, -1, 0 };
char Original[SCREEN_HEIGHT][SCREEN_WIDTH] = {
	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', '0', '0', ' ', ' ', ' ', ' ', ' ', '0', '0', '0', ' ', ' ', ' ', ' ', ' ', '0', '0', ' ' },
	{ ' ', ' ', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', ' ', ' ', '0', '0', '0', ' ' },
	{ ' ', '0', '0', '0', '0', '0', ' ', ' ', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', ' ' },
	{ ' ', '0', '0', '0', '0', ' ', ' ', ' ', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', ' ' },
	{ ' ', '0', '0', '0', ' ', '0', ' ', ' ', '0', '0', '0', ' ', ' ', '0', '0', '0', '0', '0', '0', ' ' },
	{ ' ', '0', '0', ' ', ' ', '0', ' ', '0', '0', '0', '0', '0', '0', ' ', '0', '0', '0', '0', ' ', ' ' },
	{ ' ', '0', '0', ' ', '0', '0', ' ', ' ', ' ', ' ', '0', '0', '0', ' ', '0', '0', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', '0', ' ', ' ', '0', ' ', '0', '0', '0', '0', '0', '0', ' ', '0', ' ', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0', '0', '0', '0', '0', '0', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
};

char CopyArr[SCREEN_HEIGHT][SCREEN_WIDTH];
char visited[SCREEN_HEIGHT][SCREEN_WIDTH];

struct StData
{
	int y;
	int x;
};

// 1차원 배열로 바꿔보기
StData StackArr[SCREEN_HEIGHT * SCREEN_WIDTH];

// 12.5초 ~ 12.6초
void Recur(int y, int x)
{
	visited[y][x] = 1;
	for (auto k = 0; k < 4; k++)
	{
		int ny = y + dy[k];
		int nx = x + dx[k];

		if (ny < 0 || ny >= SCREEN_HEIGHT || nx < 0 || nx >= SCREEN_WIDTH || CopyArr[ny][nx] == ' ')
		{
			continue;
		}

		if (visited[ny][nx] != 0 && CopyArr[ny][nx] != '0')
		{
			continue;
		}

		CopyArr[ny][nx] = 'x';
		Recur(ny, nx);
	}
}

// 12.2초 ~ 12.3초
void StackAndLoop(int y, int x)
{
	int top = -1;

	visited[y][x] = 1;
	StackArr[++top].y = y;
	StackArr[top].x = x;

	while (top >= 0)
	{
		y = StackArr[top].y;
		x = StackArr[top].x;
		--top;

		for (auto k = 0; k < 4; ++k)
		{
			int ny = y + dy[k];
			int nx = x + dx[k];

			if (ny < 0 || ny >= SCREEN_HEIGHT || nx < 0 || nx >= SCREEN_WIDTH || CopyArr[ny][nx] == ' ')
			{
				continue;
			}

			if (visited[ny][nx] != 0 && CopyArr[ny][nx] != '0')
			{
				continue;
			}

			CopyArr[ny][nx] = 'x';
			visited[ny][nx] = 1;
			StackArr[++top].y = ny;
			StackArr[top].x = nx;
		}
	}
}

int main()
{
	auto pastTime = clock();

	CopyArr[5][10] = 'x';
	for (auto i = 0; i < 10000000; ++i)
	{
		memset(visited, 0, sizeof(visited));
		memcpy_s(CopyArr, sizeof(CopyArr), Original, sizeof(Original));
		Recur(5, 10);
		//StackAndLoop(5, 10);
	}

	auto futureTime = clock();
	double duration = (double)(futureTime - pastTime) / CLOCKS_PER_SEC;
	cout << duration << "\n";

	return 0;
}*/