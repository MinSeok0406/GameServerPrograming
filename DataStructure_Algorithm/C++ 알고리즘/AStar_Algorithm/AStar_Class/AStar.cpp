#include "AStar.h"
#include <random>
#include <queue>
#include <utility>

const int dy[8] = { -1, 0, 1, 0, -1, 1, 1, -1 };
const int dx[8] = { 0, 1, 0, -1, 1, 1, -1, -1 };
extern int g_Best[GRID_HEIGHT][GRID_WIDTH];
extern char g_Tile[GRID_HEIGHT][GRID_WIDTH];

extern int GRID_SIZE;
extern double g_offsetX;
extern double g_offsetY;
extern HPEN g_hParentPen;
extern HPEN g_hPathPen;

AStar* AStar::_pManagerAstar = nullptr;

bool AStar::AS_Run(int sy, int sx, int ey, int ex)
{
    if (_openList.empty())
    {
        isError = true;
        return false;
    }

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
        isFindLoad = true;
        return true;
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

bool AStar::AS_Clear()
{
    while (_openList.empty() == false)
    {
        _openList.pop();
    }
    _closeList.clear();
    _endNode = nullptr;

    return true;
}

void AStar::AS_RenderParentLine(HDC hdc)
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

            int mx = (x1 + x2) / 2;
            int my = (y1 + y2) / 2;

            MoveToEx(hdc, x1, y1, NULL);
            LineTo(hdc, mx, my);
            node = node->parent;
        }
    }

    SelectObject(hdc, hOldPen);
}

void AStar::AS_RenderFinalPath(HDC hdc)
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
