#include "AStar.h"
#include <vector>
#include <map>
#include <queue>

#define DISTANCE 10
#define DIGSTANCE 14

const int dy[8] = { -1, 0, 1, 0, -1, 1, 1, -1 };
const int dx[8] = { 0, 1, 0, -1, 1, 1, -1, -1 };
std::priority_queue<Node*, std::vector<Node*>, Comp> openList;
std::map<std::pair<int, int>, int> closeList;
bool g_isrun = false;

bool AS_CreateNode(Node* parent, int g, int h, int y, int x)
{
	Node* newNode = new Node;
	newNode->f = g + h;
	newNode->g = g;
	newNode->h = h;
	newNode->y = y;
	newNode->x = x;
	newNode->parent = parent;
	openList.push(newNode);
	return true;
}

// 매개변수
// 출발지 sy, sx좌표
// 목적지 ey, ex좌표
bool AS_Run(int sy, int sx, int ey, int ex)
{
	int g = 0;
	int h = ex * DISTANCE + ey * DISTANCE;
	AS_CreateNode(nullptr, g, h, sy, sx);
	closeList[{sy, sx}]++;

	Node* endNode = nullptr;
	while (openList.empty() == false)
	{
		Node* node = openList.top();
		openList.pop();

		int y = node->y;
		int x = node->x;

		// 목적지인지 체크
		if (y == ey && x == ex)
		{
			endNode = node;
			closeList[{y, x}]++;
			break;
		}

		closeList[{y, x}]++;
		for (auto k = 0; k < 8; ++k)
		{
			int ny = y + dy[k];
			int nx = x + dx[k];

			// 갈 수 없는 길 체크
			if (ny < 0 || ny >= GRID_HEIGHT || nx < 0 || nx >= GRID_WIDTH)
			{
				continue;
			}

			// 갔던 길 체크
			if (closeList.find({ ny, nx }) != closeList.end())
			{
				continue;
			}

			// 직각 거리 & 대각선 거리 계산
			// 노드 생성해서 넣기
			if (k < 4)
			{
				int ng = node->g + DISTANCE;
				int nh = node->h - DISTANCE;
				AS_CreateNode(node, ng, nh, ny, nx);
			}
			else
			{
				int ng = node->g + DIGSTANCE;
				int nh = node->h - DISTANCE;
				AS_CreateNode(node, ng, nh, ny, nx);
			}
		}
	}

	if (endNode == nullptr)
	{
		return false;
	}

	// 최종 노드에서 처음 노드까지 선 긋기 (GUI)
	while (endNode->parent != nullptr)
	{
		// TODO
	}

	return true;
}