/*#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <deque>
using namespace std;

struct Vertex
{
	int data;
};

vector<Vertex> vertices;
vector<vector<int>> adjacent;
vector<bool> visited;

void CreateGraph()
{
	vertices.resize(6);
	adjacent = vector<vector<int>>(6);

	// 인접 리스트
	/*adjacent[0].push_back(1);
	adjacent[0].push_back(3);
	adjacent[1].push_back(0);
	adjacent[1].push_back(2);
	adjacent[1].push_back(3);
	adjacent[3].push_back(4);
	adjacent[5].push_back(4);#1#

	// 인접 행렬
	adjacent = vector<vector<int>>
	{
		{0, 1, 0, 1, 1, 0, 0},
		{1, 0, 1, 1, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 1, 0, 0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 1, 0, 0},
	};
}

void Dfs(int here)
{
	// 방문
	visited[here] = true;
	cout << "Visited : " << here << endl;

	// TODO

	// 인접 리스트 형태로 모든 인접 정점 순회
	/*for (int i = 0; i < adjacent[here].size(); i++)
	{
		int there = adjacent[here][i];
		if (visited[there] == false)
			Dfs(there);
	}#1#

	// 인접 행렬 형태로 모든 인접 정점 순회
	for (int there = 0; there < 6; there++)
	{
		if (adjacent[here][there] == 0)
			continue;

		// 아직 방문하지 않음
		if (visited[there] == false)
			Dfs(there);
	}
}

void DfsAll()
{
	visited = vector<bool>(vertices.size(), false);

	for (int i = 0; i < vertices.size(); i++)
		if (visited[i] == false)
			Dfs(i);
}

int main()
{
	CreateGraph();
	
	DfsAll();

	return 0;
}*/