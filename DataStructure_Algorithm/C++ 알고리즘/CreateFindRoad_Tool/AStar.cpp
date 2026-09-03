#include "AStar.h"

const int dy[8] = { -1, 0, 1, 0, -1, 1, 1, -1 };
const int dx[8] = { 0, 1, 0, -1, 1, 1, -1, -1 };
extern int g_Best[GRID_HEIGHT][GRID_WIDTH];
extern char g_Tile[GRID_HEIGHT][GRID_WIDTH];

AStar* AStar::_pManagerAstar = nullptr;

bool AStar::AS_Run(int sy, int sx, int ey, int ex)
{
    Node* node = _openList.top();
    _openList.pop();

    int y = node->y;
    int x = node->x;

    if (g_Tile[y][x] == (char)TILETYPE::CloseList)
    {
        return true;
    }

    _closeList[{y, x}]++;
    if (y != sy || x != sx)
    {
        g_Tile[y][x] = (char)TILETYPE::CloseList;
    }

    if (y == ey && x == ex)
    {
        _endNode = node;
        AS_FindEndNode();
        return false;
    }

    for (auto k = 0; k < 8; ++k)
    {
        int ny = y + dy[k];
        int nx = x + dx[k];

        // 갈 수 없는 길 체크
        if (ny < 0 || ny >= GRID_HEIGHT || nx < 0 || nx >= GRID_WIDTH || g_Tile[ny][nx] == (char)TILETYPE::Wall)
        {
            continue;
        }

        // 갔던 길 체크
        if (g_Tile[ny][nx] == (char)TILETYPE::CloseList)
        {
            continue;
        }

        int ng;
        int nh;
        if (k < 4)
        {
            ng = node->g + DISTANCE;
            nh = (abs(ex - nx) + abs(ey - ny)) * DISTANCE;
        }
        else
        {
            ng = node->g + DIGSTANCE;
            nh = (abs(ex - nx) + abs(ey - ny)) * DISTANCE;
        }

        if (ng < g_Best[ny][nx])
        {
            g_Best[ny][nx] = ng;
            AS_CreateNode(node, ng, nh, ny, nx);
        }
    }

    return true;
}

bool AStar::AS_CreateNode(Node* parent, int g, int h, int y, int x)
{
    Node* newNode = new Node;
    newNode->f = g + h;
    newNode->g = g;
    newNode->h = h;
    newNode->y = y;
    newNode->x = x;
    newNode->parent = parent;
    _openList.push(newNode);

    if (g_Tile[y][x] == (char)TILETYPE::Empty)
    {
        g_Tile[y][x] = (char)TILETYPE::OpenList;
    }

    return true;
}

bool AStar::AS_FindEndNode()
{
    while (_endNode != nullptr)
    {
        int x = _endNode->x;
        int y = _endNode->y;

        g_Tile[y][x] = (char)TILETYPE::FindLoad;
        _endNode = _endNode->parent;
    }

    return true;
}
