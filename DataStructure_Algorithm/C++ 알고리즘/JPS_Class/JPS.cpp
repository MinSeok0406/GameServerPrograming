#include "JPS.h"

extern int g_Best[GRID_HEIGHT][GRID_WIDTH];
extern char g_Tile[GRID_HEIGHT][GRID_WIDTH];

extern int GRID_SIZE;
extern double g_offsetX;
extern double g_offsetY;
extern HPEN g_hParentPen;
extern HPEN g_hPathPen;

JPS* JPS::_pManagerJPS = nullptr;

bool JPS::JPS_Run(int sy, int sx, int ey, int ex)
{
	if (_openList.empty())
	{
		isError = true;
		return true;
	}

	Node* node = _openList.top();
	_openList.pop();

	int y = node->y;
	int x = node->x;

	// 갔던 길 체크
	if (g_Tile[y][x] == (char)TILETYPE::CloseList)
	{
		return true;
	}

	_closeList[{y, x}]++;
	if (y != sy || x != sx)
	{
		g_Tile[y][x] = (char)TILETYPE::CloseList;
	}

	// 목적지인지 체크
	if (y == ey && x == ex)
	{
		_endNode = node;
		JPS_FindEndNode();
		isFindLoad = true;
		return true;
	}

	// JPS 노드 생성 관련 코드 들어가야함
	if (node->parent == nullptr)
	{
		// Jump 8방향 모두 호출
		// 시작 위치는 호출 노드의 그 다음 한 칸
		JPS_Jump_UL(node, node->y - 1, node->x - 1, ey, ex);
		JPS_Jump_UR(node, node->y - 1, node->x + 1, ey, ex);
		JPS_Jump_DL(node, node->y + 1, node->x - 1, ey, ex);
		JPS_Jump_DR(node, node->y + 1, node->x + 1, ey, ex);
		JPS_Jump_UU(node, node->y - 1, node->x, ey, ex);
		JPS_Jump_DD(node, node->y + 1, node->x, ey, ex);
		JPS_Jump_RR(node, node->y, node->x + 1, ey, ex);
		JPS_Jump_LL(node, node->y, node->x - 1, ey, ex);
	}
	else
	{
		// 부모 방향과 같은 Jump 호출
		// 직선
		if (node->dir & (int)DIRECTION::Jump_RR) // - 오른쪽 방향
		{
			JPS_Jump_RR(node, node->y, node->x + 1, ey, ex);
		}
		if (node->dir & (int)DIRECTION::Jump_LL) // - 왼쪽 방향
		{
			JPS_Jump_LL(node, node->y, node->x - 1, ey, ex);
		}
		if (node->dir & (int)DIRECTION::Jump_DD) // - 아랫 방향
		{
			JPS_Jump_DD(node, node->y + 1, node->x, ey, ex);
		}
		if (node->dir & (int)DIRECTION::Jump_UU) // - 윗 방향
		{
			JPS_Jump_UU(node, node->y - 1, node->x, ey, ex);
		}

		// 대각선
		if (node->dir & (int)DIRECTION::Jump_UL) // - 왼쪽 위 대각선 방향
		{
			JPS_Jump_UL(node, node->y - 1, node->x - 1, ey, ex);
		}
		if (node->dir & (int)DIRECTION::Jump_DL) // - 왼쪽 아래 대각선 방향
		{
			JPS_Jump_DL(node, node->y + 1, node->x - 1, ey, ex);
		}
		if (node->dir & (int)DIRECTION::Jump_UR) // - 오른쪽 위 대각선 방향
		{
			JPS_Jump_UR(node, node->y - 1, node->x + 1, ey, ex);
		}
		if (node->dir & (int)DIRECTION::Jump_DR) // - 오른쪽 아래 대각선 방향
		{
			JPS_Jump_DR(node, node->y + 1, node->x + 1, ey, ex);
		}
	}

	return true;
}

bool JPS::JPS_CreateNode(Node* parent, int g, int h, int y, int x, unsigned char dir)
{
	Node* newNode = new Node;
	newNode->f = g + h;
	newNode->g = g;
	newNode->h = h;
	newNode->y = y;
	newNode->x = x;
	newNode->dir = dir;
	newNode->parent = parent;
	_openList.push(newNode);

	if (g_Tile[y][x] == (char)TILETYPE::Empty)
	{
		g_Tile[y][x] = (char)TILETYPE::OpenList;
	}

	return true;
}

bool JPS::JPS_FindEndNode()
{
	Node* node = _endNode;
	while (node != nullptr)
	{
		int x = node->x;
		int y = node->y;

		g_Tile[y][x] = (char)TILETYPE::FindLoad;
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

	_endNode = nullptr;
	SelectObject(hdc, hOldPen);
}

bool JPS::JPS_Jump_UL(Node* node, int sy, int sx, int ey, int ex)
{
	int x = sx;
	int y = sy;
	while (true)
	{
		bool flag = false;
		unsigned char dir = (int)DIRECTION::Jump_UL | (int)DIRECTION::Jump_UU | (int)DIRECTION::Jump_LL;
		int nx = x - 1;
		int ny = y - 1;

		if (nx < 0 || nx >= GRID_WIDTH || ny < 0 || ny >= GRID_HEIGHT || g_Tile[ny][nx] == (char)TILETYPE::Wall)
		{
			break;
		}

		// 목적지 발견
		if (nx == ex && ny == ey)
		{
			JPS_CreateNode(node, 0, 0, ny, nx, dir);
			return true;
		}

		if ((g_Tile[y + 1][x] == (char)TILETYPE::Wall) && (g_Tile[y + 1][x - 1] == (char)TILETYPE::Empty))
		{
			dir |= (int)DIRECTION::Jump_DL;
			flag = true;
		}

		if ((g_Tile[y][x + 1] == (char)TILETYPE::Wall) && (g_Tile[y - 1][x + 1] == (char)TILETYPE::Empty))
		{
			dir |= (int)DIRECTION::Jump_UR;
			flag = true;
		}

		bool isHit = JPS_Jump_LL_Valid(y, x - 1, ey, ex) || JPS_Jump_UU_Valid(y - 1, x, ey, ex);

		if (flag || isHit)
		{
			int steps = max(std::abs(x - node->x), std::abs(y - node->y));
			int ng = node->g + DIGSTANCE * steps;
			int nh = (abs(ey - y) + abs(ex - x)) * DISTANCE;

			if (ng < g_Best[y][x])
			{
				g_Best[y][x] = ng;
				JPS_CreateNode(node, ng, nh, y, x, dir);
				break;
			}
		}

		x = nx;
		y = ny;
	}

	return true;
}

bool JPS::JPS_Jump_UR(Node* node, int sy, int sx, int ey, int ex)
{
	int x = sx;
	int y = sy;
	while (true)
	{
		bool flag = false;
		unsigned char dir = (int)DIRECTION::Jump_UR | (int)DIRECTION::Jump_UU | (int)DIRECTION::Jump_RR;
		int nx = x + 1;
		int ny = y - 1;

		if (nx < 0 || nx >= GRID_WIDTH || ny < 0 || ny >= GRID_HEIGHT || g_Tile[ny][nx] == (char)TILETYPE::Wall)
		{
			break;
		}

		// 목적지 발견
		if (nx == ex && ny == ey)
		{
			JPS_CreateNode(node, 0, 0, ny, nx, dir);
			return true;
		}

		if ((g_Tile[y][x - 1] == (char)TILETYPE::Wall) && (g_Tile[y - 1][x - 1] == (char)TILETYPE::Empty))
		{
			dir |= (int)DIRECTION::Jump_UL;
			flag = true;
		}

		if ((g_Tile[y + 1][x] == (char)TILETYPE::Wall) && (g_Tile[y + 1][x + 1] == (char)TILETYPE::Empty))
		{
			dir |= (int)DIRECTION::Jump_DR;
			flag = true;
		}

		bool isHit = JPS_Jump_UU_Valid(y - 1, x, ey, ex) || JPS_Jump_RR_Valid(y, x + 1, ey, ex);

		if (flag || isHit)
		{
			int steps = max(std::abs(x - node->x), std::abs(y - node->y));
			int ng = node->g + DIGSTANCE * steps;
			int nh = (abs(ey - y) + abs(ex - x)) * DISTANCE;

			if (ng < g_Best[y][x])
			{
				g_Best[y][x] = ng;
				JPS_CreateNode(node, ng, nh, y, x, dir);
				break;
			}
		}

		x = nx;
		y = ny;
	}

	return true;
}

bool JPS::JPS_Jump_DL(Node* node, int sy, int sx, int ey, int ex)
{
	int x = sx;
	int y = sy;
	while (true)
	{
		bool flag = false;
		unsigned char dir = (int)DIRECTION::Jump_DL | (int)DIRECTION::Jump_DD | (int)DIRECTION::Jump_LL;
		int nx = x - 1;
		int ny = y + 1;

		if (nx < 0 || nx >= GRID_WIDTH || ny < 0 || ny >= GRID_HEIGHT || g_Tile[ny][nx] == (char)TILETYPE::Wall)
		{
			break;
		}

		// 목적지 발견
		if (nx == ex && ny == ey)
		{
			JPS_CreateNode(node, 0, 0, ny, nx, dir);
			return true;
		}

		if ((g_Tile[y][x + 1] == (char)TILETYPE::Wall) && (g_Tile[y + 1][x + 1] == (char)TILETYPE::Empty))
		{
			dir |= (int)DIRECTION::Jump_DR;
			flag = true;
		}

		if ((g_Tile[y - 1][x] == (char)TILETYPE::Wall) && (g_Tile[y - 1][x - 1] == (char)TILETYPE::Empty))
		{
			dir |= (int)DIRECTION::Jump_UL;
			flag = true;
		}

		bool isHit = JPS_Jump_LL_Valid(y, x - 1, ey, ex) || JPS_Jump_DD_Valid(y + 1, x, ey, ex);

		if (flag || isHit)
		{
			int steps = max(std::abs(x - node->x), std::abs(y - node->y));
			int ng = node->g + DIGSTANCE * steps;
			int nh = (abs(ey - y) + abs(ex - x)) * DISTANCE;

			if (ng < g_Best[y][x])
			{
				g_Best[y][x] = ng;
				JPS_CreateNode(node, ng, nh, y, x, dir);
				break;
			}
		}

		x = nx;
		y = ny;
	}

	return true;
}

bool JPS::JPS_Jump_DR(Node* node, int sy, int sx, int ey, int ex)
{
	int x = sx;
	int y = sy;
	while (true)
	{
		bool flag = false;
		unsigned char dir = (int)DIRECTION::Jump_DR | (int)DIRECTION::Jump_DD | (int)DIRECTION::Jump_RR;
		int nx = x + 1;
		int ny = y + 1;

		if (nx < 0 || nx >= GRID_WIDTH || ny < 0 || ny >= GRID_HEIGHT || g_Tile[ny][nx] == (char)TILETYPE::Wall)
		{
			break;
		}

		// 목적지 발견
		if (nx == ex && ny == ey)
		{
			JPS_CreateNode(node, 0, 0, ny, nx, dir);
			return true;
		}

		if ((g_Tile[y][x - 1] == (char)TILETYPE::Wall) && (g_Tile[y + 1][x - 1] == (char)TILETYPE::Empty))
		{
			dir |= (int)DIRECTION::Jump_DL;
			flag = true;
		}

		if ((g_Tile[y - 1][x] == (char)TILETYPE::Wall) && (g_Tile[y - 1][x + 1] == (char)TILETYPE::Empty))
		{
			dir |= (int)DIRECTION::Jump_UR;
			flag = true;
		}

		bool isHit = JPS_Jump_RR_Valid(y, x + 1, ey, ex) || JPS_Jump_DD_Valid(y + 1, x, ey, ex);

		if (flag || isHit)
		{
			int steps = max(std::abs(x - node->x), std::abs(y - node->y));
			int ng = node->g + DIGSTANCE * steps;
			int nh = (abs(ey - y) + abs(ex - x)) * DISTANCE;
			if (ng < g_Best[y][x])
			{
				g_Best[y][x] = ng;
				JPS_CreateNode(node, ng, nh, y, x, dir);
				break;
			}
		}

		x = nx;
		y = ny;
	}

	return true;
}

bool JPS::JPS_Jump_UU(Node* node, int sy, int sx, int ey, int ex)
{
	int x = sx;
	int y = sy;
	while (true)
	{
		bool flag = false;
		unsigned char dir = (int)DIRECTION::Jump_UU;
		int ny = y - 1;

		if (sx < 0 || sx >= GRID_WIDTH || ny < 0 || ny >= GRID_HEIGHT || g_Tile[ny][sx] == (char)TILETYPE::Wall)
		{
			return false;
		}

		// 목적지 발견
		if (sx == ex && ny == ey)
		{
			JPS_CreateNode(node, 0, 0, ny, sx, dir);
			return true;
		}

		if (!(sx + 1 >= GRID_WIDTH) && ((g_Tile[y][sx + 1] == (char)TILETYPE::Wall) &&
			(g_Tile[ny][sx + 1] == (char)TILETYPE::Empty)))
		{
			dir |= (int)DIRECTION::Jump_UR;
			flag = true;
		}

		if (!(sx - 1 < 0) && ((g_Tile[y][sx - 1] == (char)TILETYPE::Wall) &&
			(g_Tile[ny][sx - 1] == (char)TILETYPE::Empty)))
		{
			dir |= (int)DIRECTION::Jump_UL;
			flag = true;
		}

		if (flag)
		{
			int steps = max(std::abs(x - node->x), std::abs(y - node->y));
			int ng = node->g + DISTANCE * steps;
			int nh = (abs(ey - y) + abs(ex - sx)) * DISTANCE;

			if (ng < g_Best[y][sx])
			{
				g_Best[y][sx] = ng;
				JPS_CreateNode(node, ng, nh, y, sx, dir);
				break;
			}
		}

		y = ny;
	}

	return true;
}

bool JPS::JPS_Jump_DD(Node* node, int sy, int sx, int ey, int ex)
{
	int x = sx;
	int y = sy;
	while (true)
	{
		bool flag = false;
		unsigned char dir = (int)DIRECTION::Jump_DD;
		int ny = y + 1;

		if (sx < 0 || sx >= GRID_WIDTH || ny < 0 || ny >= GRID_HEIGHT || g_Tile[ny][sx] == (char)TILETYPE::Wall)
		{
			return false;
		}

		// 목적지 발견
		if (sx == ex && ny == ey)
		{
			JPS_CreateNode(node, 0, 0, ny, sx, dir);
			return true;
		}

		if (!(sx + 1 >= GRID_WIDTH) && ((g_Tile[y][sx + 1] == (char)TILETYPE::Wall) &&
			(g_Tile[ny][sx + 1] == (char)TILETYPE::Empty)))
		{
			dir |= (int)DIRECTION::Jump_DR;
			flag = true;
		}

		if (!(sx - 1 < 0) && ((g_Tile[y][sx - 1] == (char)TILETYPE::Wall) &&
			(g_Tile[ny][sx - 1] == (char)TILETYPE::Empty)))
		{
			dir |= (int)DIRECTION::Jump_DL;
			flag = true;
		}

		if (flag)
		{
			int steps = max(std::abs(x - node->x), std::abs(y - node->y));
			int ng = node->g + DISTANCE * steps;
			int nh = (abs(ey - y) + abs(ex - sx)) * DISTANCE;

			if (ng < g_Best[y][sx])
			{
				g_Best[y][sx] = ng;
				JPS_CreateNode(node, ng, nh, y, sx, dir);
				break;
			}
		}

		y = ny;
	}

	return true;
}

bool JPS::JPS_Jump_RR(Node* node, int sy, int sx, int ey, int ex)
{
	int x = sx;
	int y = sy;
	while (true)
	{
		bool flag = false;
		unsigned char dir = (int)DIRECTION::Jump_RR;
		int nx = x + 1;

		if (nx < 0 || nx >= GRID_WIDTH || sy < 0 || sy >= GRID_HEIGHT || g_Tile[sy][nx] == (char)TILETYPE::Wall)
		{
			return false;
		}

		// 목적지 발견
		if (nx == ex && sy == ey)
		{
			JPS_CreateNode(node, 0, 0, sy, nx, dir);
			return true;
		}

		if (!(sy + 1 >= GRID_HEIGHT) && ((g_Tile[sy + 1][x] == (char)TILETYPE::Wall) &&
			(g_Tile[sy + 1][nx] == (char)TILETYPE::Empty)))
		{
			dir |= (int)DIRECTION::Jump_DR;
			flag = true;
		}

		if (!(sy - 1 < 0) && ((g_Tile[sy - 1][x] == (char)TILETYPE::Wall) &&
			(g_Tile[sy - 1][nx] == (char)TILETYPE::Empty)))
		{
			dir |= (int)DIRECTION::Jump_UR;
			flag = true;
		}

		if (flag)
		{
			int steps = max(std::abs(x - node->x), std::abs(y - node->y));
			int ng = node->g + DISTANCE * steps;
			int nh = (abs(ey - sy) + abs(ex - x)) * DISTANCE;

			if (ng < g_Best[sy][x])
			{
				g_Best[sy][x] = ng;
				JPS_CreateNode(node, ng, nh, sy, x, dir);
				break;
			}
		}

		x = nx;
	}

	return true;
}

bool JPS::JPS_Jump_LL(Node* node, int sy, int sx, int ey, int ex)
{
	int x = sx;
	int y = sy;
	while (true)
	{
		bool flag = false;
		unsigned char dir = (int)DIRECTION::Jump_LL;
		int nx = x - 1;

		if (nx < 0 || nx >= GRID_WIDTH || sy < 0 || sy >= GRID_HEIGHT || g_Tile[sy][nx] == (char)TILETYPE::Wall)
		{
			return false;
		}

		// 목적지 발견
		if (nx == ex && sy == ey)
		{
			JPS_CreateNode(node, 0, 0, sy, nx, dir);
			return true;
		}

		if (!(sy + 1 >= GRID_HEIGHT) && ((g_Tile[sy + 1][x] == (char)TILETYPE::Wall) &&
			(g_Tile[sy + 1][nx] == (char)TILETYPE::Empty)))
		{
			dir |= (int)DIRECTION::Jump_DL;
			flag = true;
		}

		if (!(sy - 1 < 0) && ((g_Tile[sy - 1][x] == (char)TILETYPE::Wall) &&
			(g_Tile[sy - 1][nx] == (char)TILETYPE::Empty)))
		{
			dir |= (int)DIRECTION::Jump_UL;
			flag = true;
		}

		if (flag)
		{
			int steps = max(std::abs(x - node->x), std::abs(y - node->y));
			int ng = node->g + DISTANCE * steps;
			int nh = (abs(ey - sy) + abs(ex - x)) * DISTANCE;

			if (ng < g_Best[sy][x])
			{
				g_Best[sy][x] = ng;
				JPS_CreateNode(node, ng, nh, sy, x, dir);
				break;
			}
		}

		x = nx;
	}

	return true;
}

bool JPS::JPS_Jump_UU_Valid(int sy, int sx, int ey, int ex)
{
	int y = sy;
	while (true)
	{
		int ny = y - 1;

		if (sx < 0 || sx >= GRID_WIDTH || ny < 0 || ny >= GRID_HEIGHT || g_Tile[ny][sx] == (char)TILETYPE::Wall)
		{
			return false;
		}

		// 목적지 발견
		if (sx == ex && ny == ey)
		{
			return true;
		}

		if (!(sx + 1 >= GRID_WIDTH) && ((g_Tile[y][sx + 1] == (char)TILETYPE::Wall) &&
			(g_Tile[ny][sx + 1] == (char)TILETYPE::Empty)))
		{
			return true;
		}

		if (!(sx - 1 < 0) && ((g_Tile[y][sx - 1] == (char)TILETYPE::Wall) &&
			(g_Tile[ny][sx - 1] == (char)TILETYPE::Empty)))
		{
			return true;
		}

		y = ny;
	}
}

bool JPS::JPS_Jump_LL_Valid(int sy, int sx, int ey, int ex)
{
	int x = sx;
	while (true)
	{
		int nx = x - 1;

		if (nx < 0 || nx >= GRID_WIDTH || sy < 0 || sy >= GRID_HEIGHT || g_Tile[sy][nx] == (char)TILETYPE::Wall)
		{
			return false;
		}

		// 목적지 발견
		if (nx == ex && sy == ey)
		{
			return true;
		}

		if (!(sy + 1 >= GRID_HEIGHT) && ((g_Tile[sy + 1][x] == (char)TILETYPE::Wall) &&
			(g_Tile[sy + 1][nx] == (char)TILETYPE::Empty)))
		{
			return true;
		}

		if (!(sy - 1 < 0) && ((g_Tile[sy - 1][x] == (char)TILETYPE::Wall) &&
			(g_Tile[sy - 1][nx] == (char)TILETYPE::Empty)))
		{
			return true;
		}

		x = nx;
	}
}

bool JPS::JPS_Jump_RR_Valid(int sy, int sx, int ey, int ex)
{
	int x = sx;
	while (true)
	{
		int nx = x + 1;

		if (nx < 0 || nx >= GRID_WIDTH || sy < 0 || sy >= GRID_HEIGHT || g_Tile[sy][nx] == (char)TILETYPE::Wall)
		{
			return false;
		}

		// 목적지 발견
		if (nx == ex && sy == ey)
		{
			return true;
		}

		if (!(sy + 1 >= GRID_HEIGHT) && ((g_Tile[sy + 1][x] == (char)TILETYPE::Wall) &&
			(g_Tile[sy + 1][nx] == (char)TILETYPE::Empty)))
		{
			return true;
		}

		if (!(sy - 1 < 0) && ((g_Tile[sy - 1][x] == (char)TILETYPE::Wall) &&
			(g_Tile[sy - 1][nx] == (char)TILETYPE::Empty)))
		{
			return true;
		}

		x = nx;
	}
}

bool JPS::JPS_Jump_DD_Valid(int sy, int sx, int ey, int ex)
{
	int y = sy;
	while (true)
	{
		int ny = y + 1;

		if (sx < 0 || sx >= GRID_WIDTH || ny < 0 || ny >= GRID_HEIGHT || g_Tile[ny][sx] == (char)TILETYPE::Wall)
		{
			return false;
		}

		// 목적지 발견
		if (sx == ex && ny == ey)
		{
			return true;
		}

		if (!(sx + 1 >= GRID_WIDTH) && ((g_Tile[y][sx + 1] == (char)TILETYPE::Wall) &&
			(g_Tile[ny][sx + 1] == (char)TILETYPE::Empty)))
		{
			return true;
		}

		if (!(sx - 1 < 0) && ((g_Tile[y][sx - 1] == (char)TILETYPE::Wall) &&
			(g_Tile[ny][sx - 1] == (char)TILETYPE::Empty)))
		{
			return true;
		}

		y = ny;
	}
}