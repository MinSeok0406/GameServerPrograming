#pragma once
#include <windows.h>
#include <map>
#include <queue>
#include <vector>

#define GRID_WIDTH 100
#define GRID_HEIGHT 50
#define DISTANCE 10
#define DIGSTANCE 14

enum class TILETYPE
{
    Empty = 0,
    Wall = 1,
    Start = 2,
    End = 3,
    OpenList = 4,
    CloseList = 5,
    FindLoad = 6
};

class AStar
{
public:
    struct Node
    {
        unsigned int f;
        unsigned int g;
        unsigned int h;
        unsigned short y;
        unsigned short x;
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
        {
            return lhs->f > rhs->f;
        }*/
    };

    static AStar* getInstance()
    {
        if (_pManagerAstar == nullptr)
        {
            _pManagerAstar = new AStar;
        }

        return _pManagerAstar;
    }

    static void destoryInstance()
    {
        if (_pManagerAstar != nullptr)
        {
            delete _pManagerAstar;
            _pManagerAstar = nullptr;
        }
    }
    
    // 길찾기 성공 실패 여부 반환
    bool AS_Run(int sy, int sx, int ey, int ex);

    // 새로운 노드 생성
    bool AS_CreateNode(Node* parent, int g, int h, int y, int x);

    // 길찾기 성공 시 마지막 노드부터 시작 노드까지 연결
    bool AS_FindEndNode();

    // 길찾기를 처음부터 다시 하기 위해서 자료구조 초기화
    bool AS_Clear();

    void AS_RenderParentLine(HDC hdc);
    void AS_RenderFinalPath(HDC hdc);

    bool isError = false;
    bool isFindLoad = false;
private:
    static AStar* _pManagerAstar;
    std::priority_queue<Node*, std::vector<Node*>, Comp> _openList;
    std::map<std::pair<int, int>, int> _closeList;
    Node* _endNode = nullptr;

    AStar() = default;
    ~AStar() = default;
};