/*#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <deque>
#include <windows.h>
using namespace std;

struct Vertex
{
	// int data;
};
vector<vector<int>> adjacent;

void CreateGraph()
{
	adjacent = vector<vector<int>>(6, vector<int>(6, -1));

	adjacent[0][1] = 15;
	adjacent[0][3] = 35;
	adjacent[1][0] = 15;
	adjacent[1][2] = 5;
	adjacent[1][3] = 10;
	adjacent[3][4] = 5;
	adjacent[5][4] = 5;
}

void Dijikstra(int here)
{
	// (vertex, cost)
	list<pair<int, int>> discovered;
	vector<int> best(6, INT32_MAX);	// 각 정점별로 지금까지 발견한 최소 거리
	vector<int> parent(6, -1);

	discovered.push_back(make_pair(here, 0));
	best[here] = 0;
	parent[here] = 0;

	while (discovered.empty() == false)
	{
		// 제일 좋은 후보 찾기
		list<pair<int, int>>::iterator bestIt;
		int bestCost = INT32_MAX;

		for (auto it = discovered.begin(); it != discovered.end(); ++it)
		{
			const int cost = it->second;

			if (cost < bestCost)
			{
				bestCost = cost;
				bestIt = it;
			}
		}

		int cost = bestIt->second;
		here = bestIt->first;
		discovered.erase(bestIt);

		// 더 짧은 경로를 뒤늦게 찾았다면 스킵
		if (best[here] < cost)
			continue;

		// 방문
		for (auto there = 0; there < 6; there++)
		{
			// 연결 X
			if (adjacent[here][there] == -1)
				continue;

			// 더 좋은 경로를 과거에 찾았으면 스킵
			int nextCost = best[here] + adjacent[here][there];
			if (nextCost >= best[there])
				continue;

			best[there] = nextCost;
			parent[there] = here;

			discovered.push_back(make_pair(there, nextCost));
		}
	}
}

int main()
{
	CreateGraph();

	Dijikstra(0);

	return 0;
}*/