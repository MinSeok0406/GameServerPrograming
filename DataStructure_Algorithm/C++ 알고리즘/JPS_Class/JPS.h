#pragma once
#include <windows.h>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>

#define DISTANCE 10
#define DIGSTANCE 14

enum class TILETYPE
{
    Empty = 0,
    Wall,
    Start,
    End,
    OpenList,
    CloseList,
    FindLoad,
};

enum class DIRECTION
{
    Jump_UL = (1 << 0),
    Jump_UR = (1 << 1),
    Jump_DL = (1 << 2),
    Jump_DR = (1 << 3),
    Jump_UU = (1 << 4),
    Jump_DD = (1 << 5),
    Jump_RR = (1 << 6),
    Jump_LL = (1 << 7),
};

class JPS
{
public:
    struct Node
    {
        unsigned int f;
        unsigned int g;
        unsigned int h;
        unsigned short y;
        unsigned short x;
        unsigned char dir;
        Node* parent;
    };

    struct Comp
    {
        // 안전 정렬
        bool operator()(const Node* lhs, const Node* rhs)
        {
            if (lhs->f == rhs->f)
            {
                if (lhs->g == rhs->g)
                {
                    return lhs->h > rhs->h;
                }
                return lhs->g > rhs->g;
            }
            return lhs->f > rhs->f;
        }

        // 불안전 정렬
        /*bool operator()(const Node* lhs, const Node* rhs)
        /*{
            /*return lhs->f > rhs->f;*/
        /*}*/
    };

    // 맵 관련 구조체가 나와서 맵ID 마다 맵 구조체 생성
    struct Map
    {
        char mapInfo[100][100];
        int BestG[100][100];
        int gridH;
        int gridW;
    };

    static JPS* getInstance()
    {
        if (_pManagerJPS == nullptr)
        {
            _pManagerJPS = new JPS;
        }

        return _pManagerJPS;
    }

    static void destroyInstance()
    {
        if (_pManagerJPS)
        {
            delete _pManagerJPS;
            _pManagerJPS = nullptr;
        }
    }

    // 길찾기 전 초기 작업
    bool JPS_Init(int mapid, int heigth, int width);

    // Tile 체크
    bool JPS_TileWall(int mapid, int y, int x, bool isDelete);
    bool JPS_TileStartEnd(int mapid, int y, int x, bool isEnd);

    // 오픈리스트에서 노드 하나 꺼내서 Jump 수행
    bool JPS_Run(int mapid, int sy, int sx, int ey, int ex);

    // 새로운 노드 생성
    bool JPS_CreateNode(int mapid, Node* parent, int g, int h, int y, int x, unsigned char dir);

    // 길찾기 성공 시 마지막 노드부터 시작 노드까지 연결
    bool JPS_FindEndNode(int mapid);

    // 길찾기를 처음부터 다시 하기 위해서 자료구조 초기화
    bool JPS_Clear();

    // Render 함수
    void JPS_RenderParentLine(HDC hdc);
    void JPS_RenderFinalPath(HDC hdc);
    void JPS_RenderBresenhamLine(int mapid, HDC hdc);

    // Jump 함수
    bool JPS_Jump_UL(int mapid, Node* node, int sy, int sx, int ey, int ex);
    bool JPS_Jump_UR(int mapid, Node* node, int sy, int sx, int ey, int ex);
    bool JPS_Jump_DL(int mapid, Node* node, int sy, int sx, int ey, int ex);
    bool JPS_Jump_DR(int mapid, Node* node, int sy, int sx, int ey, int ex);
    bool JPS_Jump_UU(int mapid, Node* node, int sy, int sx, int ey, int ex);
    bool JPS_Jump_DD(int mapid, Node* node, int sy, int sx, int ey, int ex);
    bool JPS_Jump_RR(int mapid, Node* node, int sy, int sx, int ey, int ex);
    bool JPS_Jump_LL(int mapid, Node* node, int sy, int sx, int ey, int ex);

    // 대각선 직선 검증 함수
    bool JPS_Jump_UU_Valid(int mapid, int sy, int sx, int ey, int ex);
    bool JPS_Jump_LL_Valid(int mapid, int sy, int sx, int ey, int ex);
    bool JPS_Jump_RR_Valid(int mapid, int sy, int sx, int ey, int ex);
    bool JPS_Jump_DD_Valid(int mapid, int sy, int sx, int ey, int ex);

    // 길찾기 후보정 알고리즘(브레젠햄 직선 알고리즘)
    Node* JPS_BresenhamLine(int mapid, Node* node);

    bool isError = false;
    bool isFindLoad = false;
private:
    static JPS* _pManagerJPS;
    std::priority_queue<Node*, std::vector<Node*>, Comp> _openList;
    std::map<std::pair<int, int>, int> _closeList;
    Node* _endNode = nullptr;

    std::map<int, Map> _map;

    JPS() = default;
    ~JPS() = default;
};