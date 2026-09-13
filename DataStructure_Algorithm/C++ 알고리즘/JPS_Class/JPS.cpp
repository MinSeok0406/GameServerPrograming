#include "JPS.h"

extern int GRID_SIZE;
extern double g_offsetX;
extern double g_offsetY;
extern HPEN g_hParentPen;
extern HPEN g_hPathPen;
extern HPEN g_hBresenhamPen;

JPS* JPS::_pManagerJPS = nullptr;

bool JPS::JPS_Init(int mapid, int heigth, int width)
{
	Map map;
	map.gridH = heigth;
	map.gridW = width;
	memset(map.mapInfo, (char)TILETYPE::Empty, sizeof(map.mapInfo));
	
	for (auto i = 0; i < 100; ++i)
	{
		for (auto j = 0; j < 100; ++j)
		{
			map.BestG[i][j] = INT_MAX;
		}
	}

	_map[mapid] = map;

	return true;
}

bool JPS::JPS_TileWall(int mapid, int y, int x, bool isDelete)
{
	Map& map = _map[mapid];

	if (isDelete)
	{
		map.mapInfo[y][x] = (char)TILETYPE::Empty;
	}
	else
	{
		map.mapInfo[y][x] = (char)TILETYPE::Wall;
	}
	

	return true;
}

bool JPS::JPS_TileStartEnd(int mapid, int y, int x, bool isEnd)
{
	Map& map = _map[mapid];

	if (isEnd)
	{
		map.mapInfo[y][x] = (char)TILETYPE::End;
	}
	else
	{
		map.mapInfo[y][x] = (char)TILETYPE::Start;
	}


	return true;
}

bool JPS::JPS_Run(int mapid, int sy, int sx, int ey, int ex)
{
	if (_openList.empty())
	{
		isError = true;
		return true;
	}

	Map& map = _map[mapid];
	Node* node = _openList.top();
	_openList.pop();

	int y = node->y;
	int x = node->x;

	// 갔던 길 체크
	if (map.mapInfo[y][x] == (char)TILETYPE::CloseList)
	{
		return true;
	}

	_closeList[{y, x}]++;
	if (y != sy || x != sx)
	{
		map.mapInfo[y][x] = (char)TILETYPE::CloseList;
	}

	// 목적지인지 체크
	if (y == ey && x == ex)
	{
		_endNode = node;
		JPS_FindEndNode(mapid);
		isFindLoad = true;
		return true;
	}

	// JPS 노드 생성 관련 코드 들어가야함
	if (node->parent == nullptr)
	{
		// Jump 8방향 모두 호출
		// 시작 위치는 호출 노드의 그 다음 한 칸
		map.BestG[node->y][node->x] = 0;
		JPS_Jump_UL(mapid, node, node->y - 1, node->x - 1, ey, ex);
		JPS_Jump_UR(mapid, node, node->y - 1, node->x + 1, ey, ex);
		JPS_Jump_DL(mapid, node, node->y + 1, node->x - 1, ey, ex);
		JPS_Jump_DR(mapid, node, node->y + 1, node->x + 1, ey, ex);
		JPS_Jump_UU(mapid, node, node->y - 1, node->x, ey, ex);
		JPS_Jump_DD(mapid, node, node->y + 1, node->x, ey, ex);
		JPS_Jump_RR(mapid, node, node->y, node->x + 1, ey, ex);
		JPS_Jump_LL(mapid, node, node->y, node->x - 1, ey, ex);
	}
	else
	{
		// 부모 방향과 같은 Jump 호출
		// 직선
		if (node->dir & (int)DIRECTION::Jump_RR) // - 오른쪽 방향
		{
			JPS_Jump_RR(mapid, node, node->y, node->x + 1, ey, ex);
		}
		if (node->dir & (int)DIRECTION::Jump_LL) // - 왼쪽 방향
		{
			JPS_Jump_LL(mapid, node, node->y, node->x - 1, ey, ex);
		}
		if (node->dir & (int)DIRECTION::Jump_DD) // - 아랫 방향
		{
			JPS_Jump_DD(mapid, node, node->y + 1, node->x, ey, ex);
		}
		if (node->dir & (int)DIRECTION::Jump_UU) // - 윗 방향
		{
			JPS_Jump_UU(mapid, node, node->y - 1, node->x, ey, ex);
		}

		// 대각선
		if (node->dir & (int)DIRECTION::Jump_UL) // - 왼쪽 위 대각선 방향
		{
			JPS_Jump_UL(mapid, node, node->y - 1, node->x - 1, ey, ex);
		}
		if (node->dir & (int)DIRECTION::Jump_DL) // - 왼쪽 아래 대각선 방향
		{
			JPS_Jump_DL(mapid, node, node->y + 1, node->x - 1, ey, ex);
		}
		if (node->dir & (int)DIRECTION::Jump_UR) // - 오른쪽 위 대각선 방향
		{
			JPS_Jump_UR(mapid, node, node->y - 1, node->x + 1, ey, ex);
		}
		if (node->dir & (int)DIRECTION::Jump_DR) // - 오른쪽 아래 대각선 방향
		{
			JPS_Jump_DR(mapid, node, node->y + 1, node->x + 1, ey, ex);
		}
	}

	return true;
}

bool JPS::JPS_CreateNode(int mapid, Node* parent, int g, int h, int y, int x, unsigned char dir)
{
	Map& map = _map[mapid];
	Node* newNode = new Node;
	newNode->f = g + h;
	newNode->g = g;
	newNode->h = h;
	newNode->y = y;
	newNode->x = x;
	newNode->dir = dir;
	newNode->parent = parent;
	_openList.push(newNode);

	if (map.mapInfo[y][x] == (char)TILETYPE::Empty)
	{
		map.mapInfo[y][x] = (char)TILETYPE::OpenList;
	}

	return true;
}

bool JPS::JPS_FindEndNode(int mapid)
{
	Map& map = _map[mapid];
	Node* node = _endNode;
	while (node != nullptr)
	{
		int x = node->x;
		int y = node->y;

		map.mapInfo[y][x] = (char)TILETYPE::FindLoad;
		node = node->parent;
	}

	return true;
}

bool JPS::JPS_Clear()
{
	while (_openList.empty() == false)
	{
		_openList.pop();
	}
	_closeList.clear();
	_endNode = nullptr;

	return true;
}

void JPS::JPS_RenderParentLine(HDC hdc)
{
	HPEN hOldPen = (HPEN)SelectObject(hdc, g_hParentPen);

	auto pq = _openList;
	while (pq.empty() == false)
	{
		Node* node = pq.top();
		pq.pop();

		while (node->parent != nullptr)
		{
			int x1 = (int)((node->x - g_offsetX) * GRID_SIZE + GRID_SIZE / 2);
			int y1 = (int)((node->y - g_offsetY) * GRID_SIZE + GRID_SIZE / 2);
			int x2 = (int)((node->parent->x - g_offsetX) * GRID_SIZE + GRID_SIZE / 2);
			int y2 = (int)((node->parent->y - g_offsetY) * GRID_SIZE + GRID_SIZE / 2);

			MoveToEx(hdc, x1, y1, NULL);
			LineTo(hdc, x2, y2);
			node = node->parent;
		}
	}

	SelectObject(hdc, hOldPen);
}

void JPS::JPS_RenderFinalPath(HDC hdc)
{
	if (_endNode == nullptr)
	{
		return;
	}

	HPEN hOldPen = (HPEN)SelectObject(hdc, g_hPathPen);

	Node* node = _endNode;
	int x = (int)((node->x - g_offsetX) * GRID_SIZE + GRID_SIZE / 2);
	int y = (int)((node->y - g_offsetY) * GRID_SIZE + GRID_SIZE / 2);
	MoveToEx(hdc, x, y, NULL);

	node = node->parent;
	while (node != nullptr)
	{
		x = (int)((node->x - g_offsetX) * GRID_SIZE + GRID_SIZE / 2);
		y = (int)((node->y - g_offsetY) * GRID_SIZE + GRID_SIZE / 2);
		LineTo(hdc, x, y);
		node = node->parent;
	}

	SelectObject(hdc, hOldPen);
}

void JPS::JPS_RenderBresenhamLine(int mapid, HDC hdc)
{
	if (_endNode == nullptr)
	{
		return;
	}

	HPEN hOldPen = (HPEN)SelectObject(hdc, g_hBresenhamPen);

	Node* node = _endNode;
	while (node->parent != nullptr)
	{
		Node* connectNode = JPS_BresenhamLine(mapid, node);

		int mtX = (int)((node->x - g_offsetX) * GRID_SIZE + GRID_SIZE / 2);
		int mtY = (int)((node->y - g_offsetY) * GRID_SIZE + GRID_SIZE / 2);
		MoveToEx(hdc, mtX, mtY, NULL);

		int ltX = (int)((connectNode->x - g_offsetX) * GRID_SIZE + GRID_SIZE / 2);
		int ltY = (int)((connectNode->y - g_offsetY) * GRID_SIZE + GRID_SIZE / 2);
		LineTo(hdc, ltX, ltY);

		node = connectNode;
	}

	SelectObject(hdc, hOldPen);
}

bool JPS::JPS_Jump_UL(int mapid, Node* node, int sy, int sx, int ey, int ex)
{
	Map& map = _map[mapid];
	int x = sx;
	int y = sy;
	while (true)
	{
		bool flag = false;
		unsigned char dir = (int)DIRECTION::Jump_UL | (int)DIRECTION::Jump_UU | (int)DIRECTION::Jump_LL;
		int nx = x;
		int ny = y;

		if (nx < 0 || nx >= map.gridW || ny < 0 || ny >= map.gridH || map.mapInfo[ny][nx] == (char)TILETYPE::Wall)
		{
			break;
		}

		// 목적지 발견
		if (nx == ex && ny == ey)
		{
			JPS_CreateNode(mapid, node, 0, 0, ny, nx, dir);
			return true;
		}

		if (!(y + 1 >= map.gridH || x - 1 < 0) && (map.mapInfo[y + 1][x] == (char)TILETYPE::Wall) && (map.mapInfo[y + 1][x - 1] != (char)TILETYPE::Wall))
		{
			dir |= (int)DIRECTION::Jump_DL;
			flag = true;
		}

		if (!(x + 1 >= map.gridW || y - 1 < 0) && (map.mapInfo[y][x + 1] == (char)TILETYPE::Wall) && (map.mapInfo[y - 1][x + 1] != (char)TILETYPE::Wall))
		{
			dir |= (int)DIRECTION::Jump_UR;
			flag = true;
		}

		bool isHit = JPS_Jump_LL_Valid(mapid, y, x - 1, ey, ex) || JPS_Jump_UU_Valid(mapid, y - 1, x, ey, ex);

		if (flag || isHit)
		{
			int steps = max(std::abs(x - node->x), std::abs(y - node->y));
			int ng = node->g + DIGSTANCE * steps;
			int nh = (abs(ey - y) + abs(ex - x)) * DISTANCE;

			if (ng < map.BestG[y][x])
			{
				map.BestG[y][x] = ng;
				JPS_CreateNode(mapid, node, ng, nh, y, x, dir);
				break;
			}
		}

		x = nx - 1;
		y = ny - 1;
	}

	return true;
}

bool JPS::JPS_Jump_UR(int mapid, Node* node, int sy, int sx, int ey, int ex)
{
	Map& map = _map[mapid];
	int x = sx;
	int y = sy;
	while (true)
	{
		bool flag = false;
		unsigned char dir = (int)DIRECTION::Jump_UR | (int)DIRECTION::Jump_UU | (int)DIRECTION::Jump_RR;
		int nx = x;
		int ny = y;

		if (nx < 0 || nx >= map.gridW || ny < 0 || ny >= map.gridH || map.mapInfo[ny][nx] == (char)TILETYPE::Wall)
		{
			break;
		}

		// 목적지 발견
		if (nx == ex && ny == ey)
		{
			JPS_CreateNode(mapid, node, 0, 0, ny, nx, dir);
			return true;
		}

		if (!(x - 1 < 0 || y - 1 < 0) && (map.mapInfo[y][x - 1] == (char)TILETYPE::Wall) && (map.mapInfo[y - 1][x - 1] != (char)TILETYPE::Wall))
		{
			dir |= (int)DIRECTION::Jump_UL;
			flag = true;
		}

		if (!(x + 1 >= map.gridW || y + 1 >= map.gridH) && (map.mapInfo[y + 1][x] == (char)TILETYPE::Wall) && (map.mapInfo[y + 1][x + 1] != (char)TILETYPE::Wall))
		{
			dir |= (int)DIRECTION::Jump_DR;
			flag = true;
		}

		bool isHit = JPS_Jump_UU_Valid(mapid, y - 1, x, ey, ex) || JPS_Jump_RR_Valid(mapid, y, x + 1, ey, ex);

		if (flag || isHit)
		{
			int steps = max(std::abs(x - node->x), std::abs(y - node->y));
			int ng = node->g + DIGSTANCE * steps;
			int nh = (abs(ey - y) + abs(ex - x)) * DISTANCE;

			if (ng < map.BestG[y][x])
			{
				map.BestG[y][x] = ng;
				JPS_CreateNode(mapid, node, ng, nh, y, x, dir);
				break;
			}
		}

		x = nx + 1;
		y = ny - 1;
	}

	return true;
}

bool JPS::JPS_Jump_DL(int mapid, Node* node, int sy, int sx, int ey, int ex)
{
	Map& map = _map[mapid];
	int x = sx;
	int y = sy;
	while (true)
	{
		bool flag = false;
		unsigned char dir = (int)DIRECTION::Jump_DL | (int)DIRECTION::Jump_DD | (int)DIRECTION::Jump_LL;
		int nx = x;
		int ny = y;

		if (nx < 0 || nx >= map.gridW || ny < 0 || ny >= map.gridH || map.mapInfo[ny][nx] == (char)TILETYPE::Wall)
		{
			break;
		}

		// 목적지 발견
		if (nx == ex && ny == ey)
		{
			JPS_CreateNode(mapid, node, 0, 0, ny, nx, dir);
			return true;
		}

		if (!(x + 1 >= map.gridW || y + 1 >= map.gridH) && (map.mapInfo[y][x + 1] == (char)TILETYPE::Wall) && (map.mapInfo[y + 1][x + 1] != (char)TILETYPE::Wall))
		{
			dir |= (int)DIRECTION::Jump_DR;
			flag = true;
		}

		if (!(x - 1 < 0 || y - 1 < 0) && (map.mapInfo[y - 1][x] == (char)TILETYPE::Wall) && (map.mapInfo[y - 1][x - 1] != (char)TILETYPE::Wall))
		{
			dir |= (int)DIRECTION::Jump_UL;
			flag = true;
		}

		bool isHit = JPS_Jump_LL_Valid(mapid, y, x - 1, ey, ex) || JPS_Jump_DD_Valid(mapid, y + 1, x, ey, ex);

		if (flag || isHit)
		{
			int steps = max(std::abs(x - node->x), std::abs(y - node->y));
			int ng = node->g + DIGSTANCE * steps;
			int nh = (abs(ey - y) + abs(ex - x)) * DISTANCE;

			if (ng < map.BestG[y][x])
			{
				map.BestG[y][x] = ng;
				JPS_CreateNode(mapid, node, ng, nh, y, x, dir);
				break;
			}
		}

		x = nx - 1;
		y = ny + 1;
	}

	return true;
}

bool JPS::JPS_Jump_DR(int mapid, Node* node, int sy, int sx, int ey, int ex)
{
	Map& map = _map[mapid];
	int x = sx;
	int y = sy;
	while (true)
	{
		bool flag = false;
		unsigned char dir = (int)DIRECTION::Jump_DR | (int)DIRECTION::Jump_DD | (int)DIRECTION::Jump_RR;
		int nx = x;
		int ny = y;

		if (nx < 0 || nx >= map.gridW || ny < 0 || ny >= map.gridH || map.mapInfo[ny][nx] == (char)TILETYPE::Wall)
		{
			break;
		}

		// 목적지 발견
		if (nx == ex && ny == ey)
		{
			JPS_CreateNode(mapid, node, 0, 0, ny, nx, dir);
			return true;
		}

		if (!(x - 1 < 0 || y + 1 >= map.gridH) && (map.mapInfo[y][x - 1] == (char)TILETYPE::Wall) && (map.mapInfo[y + 1][x - 1] != (char)TILETYPE::Wall))
		{
			dir |= (int)DIRECTION::Jump_DL;
			flag = true;
		}

		if (!(x + 1 >= map.gridW || y - 1 < 0) && (map.mapInfo[y - 1][x] == (char)TILETYPE::Wall) && (map.mapInfo[y - 1][x + 1] != (char)TILETYPE::Wall))
		{
			dir |= (int)DIRECTION::Jump_UR;
			flag = true;
		}

		bool isHit = JPS_Jump_RR_Valid(mapid, y, x + 1, ey, ex) || JPS_Jump_DD_Valid(mapid, y + 1, x, ey, ex);

		if (flag || isHit)
		{
			int steps = max(std::abs(x - node->x), std::abs(y - node->y));
			int ng = node->g + DIGSTANCE * steps;
			int nh = (abs(ey - y) + abs(ex - x)) * DISTANCE;
			if (ng < map.BestG[y][x])
			{
				map.BestG[y][x] = ng;
				JPS_CreateNode(mapid, node, ng, nh, y, x, dir);
				break;
			}
		}

		x = nx + 1;
		y = ny + 1;
	}

	return true;
}

bool JPS::JPS_Jump_UU(int mapid, Node* node, int sy, int sx, int ey, int ex)
{
	Map& map = _map[mapid];
	int x = sx;
	int y = sy;
	while (true)
	{
		bool flag = false;
		unsigned char dir = (int)DIRECTION::Jump_UU;
		int ny = y;

		if (sx < 0 || sx >= map.gridW || ny < 0 || ny >= map.gridH || map.mapInfo[ny][sx] == (char)TILETYPE::Wall)
		{
			return false;
		}

		// 목적지 발견
		if (sx == ex && ny == ey)
		{
			JPS_CreateNode(mapid, node, 0, 0, ny, sx, dir);
			return true;
		}

		if (!(sx + 1 >= map.gridW) && ((map.mapInfo[y][sx + 1] == (char)TILETYPE::Wall) &&
			(map.mapInfo[y - 1][sx + 1] != (char)TILETYPE::Wall)))
		{
			dir |= (int)DIRECTION::Jump_UR;
			flag = true;
		}

		if (!(sx - 1 < 0) && ((map.mapInfo[y][sx - 1] == (char)TILETYPE::Wall) &&
			(map.mapInfo[y - 1][sx - 1] != (char)TILETYPE::Wall)))
		{
			dir |= (int)DIRECTION::Jump_UL;
			flag = true;
		}

		if (flag)
		{
			int steps = max(std::abs(x - node->x), std::abs(y - node->y));
			int ng = node->g + DISTANCE * steps;
			int nh = (abs(ey - y) + abs(ex - sx)) * DISTANCE;

			if (ng < map.BestG[y][sx])
			{
				map.BestG[y][sx] = ng;
				JPS_CreateNode(mapid, node, ng, nh, y, sx, dir);
				break;
			}
		}

		y = ny - 1;
	}

	return true;
}

bool JPS::JPS_Jump_DD(int mapid, Node* node, int sy, int sx, int ey, int ex)
{
	Map& map = _map[mapid];
	int x = sx;
	int y = sy;
	while (true)
	{
		bool flag = false;
		unsigned char dir = (int)DIRECTION::Jump_DD;
		int ny = y;

		if (sx < 0 || sx >= map.gridW || ny < 0 || ny >= map.gridH || map.mapInfo[ny][sx] == (char)TILETYPE::Wall)
		{
			return false;
		}

		// 목적지 발견
		if (sx == ex && ny == ey)
		{
			JPS_CreateNode(mapid, node, 0, 0, ny, sx, dir);
			return true;
		}

		if (!(sx + 1 >= map.gridW) && ((map.mapInfo[y][sx + 1] == (char)TILETYPE::Wall) &&
			(map.mapInfo[y + 1][sx + 1] != (char)TILETYPE::Wall)))
		{
			dir |= (int)DIRECTION::Jump_DR;
			flag = true;
		}

		if (!(sx - 1 < 0) && ((map.mapInfo[y][sx - 1] == (char)TILETYPE::Wall) &&
			(map.mapInfo[y + 1][sx - 1] != (char)TILETYPE::Wall)))
		{
			dir |= (int)DIRECTION::Jump_DL;
			flag = true;
		}

		if (flag)
		{
			int steps = max(std::abs(x - node->x), std::abs(y - node->y));
			int ng = node->g + DISTANCE * steps;
			int nh = (abs(ey - y) + abs(ex - sx)) * DISTANCE;

			if (ng < map.BestG[y][sx])
			{
				map.BestG[y][sx] = ng;
				JPS_CreateNode(mapid, node, ng, nh, y, sx, dir);
				break;
			}
		}

		y = ny + 1;
	}

	return true;
}

bool JPS::JPS_Jump_RR(int mapid, Node* node, int sy, int sx, int ey, int ex)
{
	Map& map = _map[mapid];
	int x = sx;
	int y = sy;
	while (true)
	{
		bool flag = false;
		unsigned char dir = (int)DIRECTION::Jump_RR;
		int nx = x;

		if (nx < 0 || nx >= map.gridW || sy < 0 || sy >= map.gridH || map.mapInfo[sy][nx] == (char)TILETYPE::Wall)
		{
			return false;
		}

		// 목적지 발견
		if (nx == ex && sy == ey)
		{
			JPS_CreateNode(mapid, node, 0, 0, sy, nx, dir);
			return true;
		}

		if (!(sy + 1 >= map.gridH) && ((map.mapInfo[sy + 1][x] == (char)TILETYPE::Wall) &&
			(map.mapInfo[sy + 1][x + 1] != (char)TILETYPE::Wall)))
		{
			dir |= (int)DIRECTION::Jump_DR;
			flag = true;
		}

		if (!(sy - 1 < 0) && ((map.mapInfo[sy - 1][x] == (char)TILETYPE::Wall) &&
			(map.mapInfo[sy - 1][x + 1] != (char)TILETYPE::Wall)))
		{
			dir |= (int)DIRECTION::Jump_UR;
			flag = true;
		}

		if (flag)
		{
			int steps = max(std::abs(x - node->x), std::abs(y - node->y));
			int ng = node->g + DISTANCE * steps;
			int nh = (abs(ey - sy) + abs(ex - x)) * DISTANCE;

			if (ng < map.BestG[sy][x])
			{
				map.BestG[sy][x] = ng;
				JPS_CreateNode(mapid, node, ng, nh, sy, x, dir);
				break;
			}
		}

		x = nx + 1;
	}

	return true;
}

bool JPS::JPS_Jump_LL(int mapid, Node* node, int sy, int sx, int ey, int ex)
{
	Map& map = _map[mapid];
	int x = sx;
	int y = sy;
	while (true)
	{
		bool flag = false;
		unsigned char dir = (int)DIRECTION::Jump_LL;
		int nx = x;

		if (nx < 0 || nx >= map.gridW || sy < 0 || sy >= map.gridH || map.mapInfo[sy][nx] == (char)TILETYPE::Wall)
		{
			return false;
		}

		// 목적지 발견
		if (nx == ex && sy == ey)
		{
			JPS_CreateNode(mapid, node, 0, 0, sy, nx, dir);
			return true;
		}

		if (!(sy + 1 >= map.gridH) && ((map.mapInfo[sy + 1][x] == (char)TILETYPE::Wall) &&
			(map.mapInfo[sy + 1][x - 1] != (char)TILETYPE::Wall)))
		{
			dir |= (int)DIRECTION::Jump_DL;
			flag = true;
		}

		if (!(sy - 1 < 0) && ((map.mapInfo[sy - 1][x] == (char)TILETYPE::Wall) &&
			(map.mapInfo[sy - 1][x - 1] != (char)TILETYPE::Wall)))
		{
			dir |= (int)DIRECTION::Jump_UL;
			flag = true;
		}

		if (flag)
		{
			int steps = max(std::abs(x - node->x), std::abs(y - node->y));
			int ng = node->g + DISTANCE * steps;
			int nh = (abs(ey - sy) + abs(ex - x)) * DISTANCE;

			if (ng < map.BestG[sy][x])
			{
				map.BestG[sy][x] = ng;
				JPS_CreateNode(mapid, node, ng, nh, sy, x, dir);
				break;
			}
		}

		x = nx - 1;
	}

	return true;
}

bool JPS::JPS_Jump_UU_Valid(int mapid, int sy, int sx, int ey, int ex)
{
	Map& map = _map[mapid];
	int y = sy;
	while (true)
	{
		int ny = y;

		if (sx < 0 || sx >= map.gridW || ny < 0 || ny >= map.gridH || map.mapInfo[ny][sx] == (char)TILETYPE::Wall)
		{
			return false;
		}

		// 목적지 발견
		if (sx == ex && ny == ey)
		{
			return true;
		}

		if (!(sx + 1 >= map.gridW) && ((map.mapInfo[y][sx + 1] == (char)TILETYPE::Wall) &&
			(map.mapInfo[y - 1][sx + 1] != (char)TILETYPE::Wall)))
		{
			return true;
		}

		if (!(sx - 1 < 0) && ((map.mapInfo[y][sx - 1] == (char)TILETYPE::Wall) &&
			(map.mapInfo[y - 1][sx - 1] != (char)TILETYPE::Wall)))
		{
			return true;
		}

		y = ny - 1;
	}
}

bool JPS::JPS_Jump_LL_Valid(int mapid, int sy, int sx, int ey, int ex)
{
	Map& map = _map[mapid];
	int x = sx;
	while (true)
	{
		int nx = x;

		if (nx < 0 || nx >= map.gridW || sy < 0 || sy >= map.gridH || map.mapInfo[sy][nx] == (char)TILETYPE::Wall)
		{
			return false;
		}

		// 목적지 발견
		if (nx == ex && sy == ey)
		{
			return true;
		}

		if (!(sy + 1 >= map.gridH) && ((map.mapInfo[sy + 1][x] == (char)TILETYPE::Wall) &&
			(map.mapInfo[sy + 1][x - 1] != (char)TILETYPE::Wall)))
		{
			return true;
		}

		if (!(sy - 1 < 0) && ((map.mapInfo[sy - 1][x] == (char)TILETYPE::Wall) &&
			(map.mapInfo[sy - 1][x - 1] != (char)TILETYPE::Wall)))
		{
			return true;
		}

		x = nx - 1;
	}
}

bool JPS::JPS_Jump_RR_Valid(int mapid, int sy, int sx, int ey, int ex)
{
	Map& map = _map[mapid];
	int x = sx;
	while (true)
	{
		int nx = x;

		if (nx < 0 || nx >= map.gridW || sy < 0 || sy >= map.gridH || map.mapInfo[sy][nx] == (char)TILETYPE::Wall)
		{
			return false;
		}

		// 목적지 발견
		if (nx == ex && sy == ey)
		{
			return true;
		}

		if (!(sy + 1 >= map.gridH) && ((map.mapInfo[sy + 1][x] == (char)TILETYPE::Wall) &&
			(map.mapInfo[sy + 1][x + 1] != (char)TILETYPE::Wall)))
		{
			return true;
		}

		if (!(sy - 1 < 0) && ((map.mapInfo[sy - 1][x] == (char)TILETYPE::Wall) &&
			(map.mapInfo[sy - 1][x + 1] != (char)TILETYPE::Wall)))
		{
			return true;
		}

		x = nx + 1;
	}
}

bool JPS::JPS_Jump_DD_Valid(int mapid, int sy, int sx, int ey, int ex)
{
	Map& map = _map[mapid];
	int y = sy;
	while (true)
	{
		int ny = y;

		if (sx < 0 || sx >= map.gridW || ny < 0 || ny >= map.gridH || map.mapInfo[ny][sx] == (char)TILETYPE::Wall)
		{
			return false;
		}

		// 목적지 발견
		if (sx == ex && ny == ey)
		{
			return true;
		}

		if (!(sx + 1 >= map.gridW) && ((map.mapInfo[y][sx + 1] == (char)TILETYPE::Wall) &&
			(map.mapInfo[y + 1][sx + 1] != (char)TILETYPE::Wall)))
		{
			return true;
		}

		if (!(sx - 1 < 0) && ((map.mapInfo[y][sx - 1] == (char)TILETYPE::Wall) &&
			(map.mapInfo[y + 1][sx - 1] != (char)TILETYPE::Wall)))
		{
			return true;
		}

		y = ny + 1;
	}
}

JPS::Node* JPS::JPS_BresenhamLine(int mapid, Node* node)
{
	Map& map = _map[mapid];
	Node* presentNode = node;
	Node* connectNode = node->parent;

	Node* validNode = connectNode;
	while (connectNode != nullptr)
	{
		int sy = presentNode->y;
		int sx = presentNode->x;
		int ey = connectNode->y;
		int ex = connectNode->x;

		int dy = std::abs(ey - sy);
		int dx = std::abs(ex - sx);

		// 진행 방향
		int addY = (sy < ey) ? 1 : -1;
		int addX = (sx < ex) ? 1 : -1;

		// 오차항
		int err = dx - dy;

		int y = sy;
		int x = sx;
		bool blocked = false;

		while (true)
		{
			if (x == ex && y == ey)
			{
				break;
			}

			int e2 = err * 2;

			if (e2 > -dy)
			{
				err -= dy;
				x += addX;
			}

			if (e2 < dx)
			{
				err += dx;
				y += addY;
			}

			if (x < 0 || x >= map.gridW || y < 0 || y >= map.gridH || map.mapInfo[y][x] == (char)TILETYPE::Wall)
			{
				blocked = true;
				break;
			}
		}

		if (blocked)
		{
			break;
		}

		validNode = connectNode;
		connectNode = connectNode->parent;
	}

	return validNode;
}