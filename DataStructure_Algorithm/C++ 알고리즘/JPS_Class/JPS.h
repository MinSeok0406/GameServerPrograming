#pragma once
#include <windows.h>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>

#define GRID_WIDTH 100
#define GRID_HEIGHT 50
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

    // 오픈리스트에서 노드 하나 꺼내서 Jump 수행
    bool JPS_Run(int sy, int sx, int ey, int ex);

    // 새로운 노드 생성
    bool JPS_CreateNode(Node* parent, int g, int h, int y, int x, unsigned char dir);

    // 길찾기 성공 시 마지막 노드부터 시작 노드까지 연결
    bool JPS_FindEndNode();

    // 길찾기를 처음부터 다시 하기 위해서 자료구조 초기화
    bool JPS_Clear();

    // Render 함수
    void JPS_RenderParentLine(HDC hdc);
    void JPS_RenderFinalPath(HDC hdc);

    // Jump 함수
    bool JPS_Jump_UL(Node* node, int sy, int sx, int ey, int ex);
    bool JPS_Jump_UR(Node* node, int sy, int sx, int ey, int ex);
    bool JPS_Jump_DL(Node* node, int sy, int sx, int ey, int ex);
    bool JPS_Jump_DR(Node* node, int sy, int sx, int ey, int ex);
    bool JPS_Jump_UU(Node* node, int sy, int sx, int ey, int ex);
    bool JPS_Jump_DD(Node* node, int sy, int sx, int ey, int ex);
    bool JPS_Jump_RR(Node* node, int sy, int sx, int ey, int ex);
    bool JPS_Jump_LL(Node* node, int sy, int sx, int ey, int ex);

    // 대각선 직선 검증 함수
    bool JPS_Jump_UU_Valid(int sy, int sx, int ey, int ex);
    bool JPS_Jump_LL_Valid(int sy, int sx, int ey, int ex);
    bool JPS_Jump_RR_Valid(int sy, int sx, int ey, int ex);
    bool JPS_Jump_DD_Valid(int sy, int sx, int ey, int ex);

    bool isError = false;
    bool isFindLoad = false;
private:
    static JPS* _pManagerJPS;
    std::priority_queue<Node*, std::vector<Node*>, Comp> _openList;
    std::map<std::pair<int, int>, int> _closeList;
    Node* _endNode = nullptr;

    JPS() = default;
    ~JPS() = default;
};