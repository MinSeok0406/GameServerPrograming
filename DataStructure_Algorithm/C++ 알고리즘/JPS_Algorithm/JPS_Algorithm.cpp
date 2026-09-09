// JPS_Algorithm.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "JPS_Algorithm.h"
#include <vector>
#include <map>
#include <queue>
#include <algorithm>

#define MAX_LOADSTRING 100

#define GRID_WIDTH 100
#define GRID_HEIGHT 50
#define DISTANCE 10
#define DIGSTANCE 14

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
    {
        return lhs->f > rhs->f;
    }*/
};

const int dy[8] = { -1, 0, 1, 0, -1, 1, 1, -1 };
const int dx[8] = { 0, 1, 0, -1, 1, 1, -1, -1 };
int g_Best[GRID_HEIGHT][GRID_WIDTH];
std::priority_queue<Node*, std::vector<Node*>, Comp> openList;
std::map<std::pair<int, int>, int> closeList;
Node* g_PathEndNode = nullptr;
bool g_isrun = false;

Node* JPS_CreateNode(Node* parent, int g, int h, int y, int x, unsigned char dir);
Node* JPS_AllocNode(Node* parent, int g, int h, int y, int x, unsigned char dir);
bool JPS_CommitNode(Node* node);
bool JPS_FindEndNode(Node* node);
bool JPS_Update(Node* node, int ey, int ex);

// Jump 함수
bool Jump_UL(Node* node, int sy, int sx, int ey, int ex);
bool Jump_UR(Node* node, int sy, int sx, int ey, int ex);
bool Jump_DL(Node* node, int sy, int sx, int ey, int ex);
bool Jump_DR(Node* node, int sy, int sx, int ey, int ex);
bool Jump_UU(Node* node, int sy, int sx, int ey, int ex);
bool Jump_DD(Node* node, int sy, int sx, int ey, int ex);
bool Jump_RR(Node* node, int sy, int sx, int ey, int ex);
bool Jump_LL(Node* node, int sy, int sx, int ey, int ex);

// 대각선 직선 검증 함수
bool JPS_Jump_UU_Valid(int sy, int sx, int ey, int ex);
bool JPS_Jump_LL_Valid(int sy, int sx, int ey, int ex);
bool JPS_Jump_RR_Valid(int sy, int sx, int ey, int ex);
bool JPS_Jump_DD_Valid(int sy, int sx, int ey, int ex);

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
    Jump_UL = 0,
    Jump_UU,
    Jump_UR,
    Jump_DL,
    Jump_DR,
    Jump_DD,
    Jump_RR,
    Jump_LL,
};

HPEN g_hGridPen;
HPEN g_hParentPen;
HPEN g_hPathPen;
HBRUSH g_hBrushEmpty;
HBRUSH g_hBrushWall;
HBRUSH g_hBrushStart;
HBRUSH g_hBrushEnd;
HBRUSH g_hBrushOpenList;
HBRUSH g_hBrushCloseList;
HBRUSH g_hBrushFindLoad;

// 0 -> 벽 없음, 1 -> 벽 있음, 2 -> 출발지, 3 -> 목적지
char g_Tile[GRID_HEIGHT][GRID_WIDTH];
bool g_bStartDrag = false;
bool g_bEndDrag = false;
bool g_bWallErase = false;
bool g_bWallDrag = false;
bool g_bStart = true;
int GRID_SIZE = 16;
double g_offsetX = 0.0;
double g_offsetY = 0.0;
int g_StartX = -1;
int g_StartY = -1;
int g_EndX = -1;
int g_EndY = -1;

bool ScreenToTile(int xPos, int yPos, int* outTileX, int* outTileY);
void RenderGrid(HDC hdc);
void RenderParentLine(HDC hdc);
void RenderFinalPath(HDC hdc);
void RenderObstacle(HDC hdc);

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    AllocConsole(); // 콘솔창 생성

    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout); // stdout을 방금 만든 콘솔로 연결
    freopen_s(&fp, "CONOUT$", "w", stderr); // stderr도 연결
    freopen_s(&fp, "CONIN$", "r", stdin);  // 필요하면 입력도 연결

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_JPSALGORITHM, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_JPSALGORITHM));

    for (auto i = 0; i < GRID_HEIGHT; ++i)
    {
        for (auto j = 0; j < GRID_WIDTH; ++j)
        {
            g_Best[i][j] = INT_MAX;
        }
    }

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_JPSALGORITHM));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_JPSALGORITHM);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    switch (message)
    {
    case WM_KEYUP:
        if (wParam == VK_SPACE) // 단계적 진행
        {
            if (!g_bStart)
            {
                while (openList.empty() == false)
                {
                    openList.pop();
                }
                closeList.clear();

                g_PathEndNode = nullptr;
                g_Best[g_StartY][g_StartX] = 0;
                unsigned char dir = 0;
                for (auto i = 0; i < 8; ++i)
                {
                    dir |= (1 << i);
                }
                int g = 0;
                int h = (abs(g_EndY - g_StartY) + abs(g_EndX - g_StartX)) * DISTANCE;
                JPS_CreateNode(nullptr, g, h, g_StartY, g_StartX, dir);
                g_bStart = true;
            }

            if (g_StartX != -1 && g_StartY != -1 && g_EndX != -1 && g_EndY != -1)
            {
                // openList, closeList 그리드 표현
                if (openList.empty() == false)
                {
                    Node* node = openList.top();
                    openList.pop();
                    JPS_Update(node, g_EndY, g_EndX);
                    InvalidateRect(hWnd, NULL, true);
                }
            }
        }
        else if (wParam == VK_TAB)  // 한 번에 진행
        {
            if (!g_bStart)
            {
                while (openList.empty() == false)
                {
                    openList.pop();
                }
                closeList.clear();

                g_PathEndNode = nullptr;
                g_Best[g_StartY][g_StartX] = 0;
                unsigned char dir = 0;
                for (auto i = 0; i < 8; ++i)
                {
                    dir |= (1 << i);
                }
                int g = 0;
                int h = (abs(g_EndY - g_StartY) + abs(g_EndX - g_StartX)) * DISTANCE;
                JPS_CreateNode(nullptr, g, h, g_StartY, g_StartX, dir);
                g_bStart = true;
            }

            if (g_StartX != -1 && g_StartY != -1 && g_EndX != -1 && g_EndY != -1)
            {
                while (openList.empty() == false)
                {
                    Node* node = openList.top();
                    openList.pop();
                    if (!JPS_Update(node, g_EndY, g_EndX))
                    {
                        InvalidateRect(hWnd, NULL, true);
                        break;
                    }
                    InvalidateRect(hWnd, NULL, true);
                }
            }
        }
        break;
    case WM_LBUTTONDOWN:    // 출발지 및 목적지 생성
        g_bStartDrag = true;
        {
            int iTileX;
            int iTileY;
            if (ScreenToTile(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), &iTileX, &iTileY))
            {
                if (g_bEndDrag)
                {
                    if (g_EndX != -1 || g_EndY != -1)
                    {
                        g_Tile[g_EndY][g_EndX] = (char)TILETYPE::Empty;
                    }

                    g_Tile[iTileY][iTileX] = (char)TILETYPE::End;
                    g_EndY = iTileY;
                    g_EndX = iTileX;
                    g_bStart = false;
                }
                else
                {
                    if (g_StartX != -1 || g_StartY != -1)
                    {
                        // 처음 시작 시 노드들 초기화
                        memset(g_Tile, 0, sizeof(g_Tile));
                        g_Tile[g_StartY][g_StartX] = (char)TILETYPE::Empty;
                    }

                    g_Tile[iTileY][iTileX] = (char)TILETYPE::Start;
                    g_StartY = iTileY;
                    g_StartX = iTileX;
                }

                InvalidateRect(hWnd, NULL, true);
            }
        }
        break;
    case WM_LBUTTONUP:
        g_bStartDrag = false;
        g_bEndDrag = !g_bEndDrag;
        break;
    case WM_RBUTTONDOWN:    // 벽 생성
        g_bWallDrag = true;
        {
            int iTileX;
            int iTileY;
            if (ScreenToTile(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), &iTileX, &iTileY))
            {
                g_bWallErase = (g_Tile[iTileY][iTileX] == (char)TILETYPE::Wall);
            }
        }
        break;
    case WM_RBUTTONUP:
        g_bWallDrag = false;
        break;
    case WM_MOUSEWHEEL:
    {
        POINT pt;
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        ScreenToClient(hWnd, &pt);

        double worldX = pt.x / (double)GRID_SIZE + g_offsetX;
        double worldY = pt.y / (double)GRID_SIZE + g_offsetY;

        short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        // 휠 위로 이동
        if (zDelta > 0)
        {
            GRID_SIZE += 1;
        }
        else // 아래로 이동
        {
            GRID_SIZE -= 1;
            if (GRID_SIZE < 16)
            {
                GRID_SIZE = 16;
            }
        }

        g_offsetX = worldX - pt.x / (double)GRID_SIZE;
        g_offsetY = worldY - pt.y / (double)GRID_SIZE;

        InvalidateRect(hWnd, NULL, true);
    }
    break;
    case WM_MOUSEMOVE:
    {
        if (g_bWallDrag && !g_bStartDrag)
        {
            int iTileX;
            int iTileY;

            if (ScreenToTile(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), &iTileX, &iTileY))
            {
                g_Tile[iTileY][iTileX] = !g_bWallErase;
                InvalidateRect(hWnd, NULL, true);
            }
        }
    }
    break;
    case WM_CREATE:
        g_hGridPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
        g_hParentPen = CreatePen(PS_SOLID, 1, RGB(150, 150, 200));
        g_hPathPen = CreatePen(PS_SOLID, 2, RGB(255, 150, 200));
        g_hBrushEmpty = CreateSolidBrush(RGB(255, 255, 255));
        g_hBrushWall = CreateSolidBrush(RGB(100, 100, 100));
        g_hBrushStart = CreateSolidBrush(RGB(0, 200, 0));
        g_hBrushEnd = CreateSolidBrush(RGB(200, 0, 0));
        g_hBrushOpenList = CreateSolidBrush(RGB(0, 0, 200));
        g_hBrushCloseList = CreateSolidBrush(RGB(255, 255, 0));
        g_hBrushFindLoad = CreateSolidBrush(RGB(255, 0, 255));
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps);
        RenderObstacle(hdc);
        RenderGrid(hdc);
        RenderParentLine(hdc);
        RenderFinalPath(hdc);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        DeleteObject(g_hBrushEmpty);
        DeleteObject(g_hBrushWall);
        DeleteObject(g_hBrushStart);
        DeleteObject(g_hBrushEnd);
        DeleteObject(g_hBrushOpenList);
        DeleteObject(g_hBrushCloseList);
        DeleteObject(g_hBrushFindLoad);
        DeleteObject(g_hGridPen);
        DeleteObject(g_hParentPen);
        DeleteObject(g_hPathPen);
        //g_Astar->destoryInstance();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

bool ScreenToTile(int xPos, int yPos, int* outTileX, int* outTileY)
{
    int tileX = (int)floor(xPos / (double)GRID_SIZE + g_offsetX);
    int tileY = (int)floor(yPos / (double)GRID_SIZE + g_offsetY);

    if (tileX < 0 || tileX >= GRID_WIDTH || tileY < 0 || tileY >= GRID_HEIGHT)
    {
        return false;
    }

    *outTileX = tileX;
    *outTileY = tileY;
    return true;
}

void RenderGrid(HDC hdc)
{
    HPEN hOldPen = (HPEN)SelectObject(hdc, g_hGridPen);

    for (int iCntW = 0; iCntW <= GRID_WIDTH; ++iCntW)
    {
        int iX = (int)((iCntW - g_offsetX) * GRID_SIZE);
        MoveToEx(hdc, iX, (int)((0 - g_offsetY) * GRID_SIZE), NULL);
        LineTo(hdc, iX, (int)((GRID_HEIGHT - g_offsetY) * GRID_SIZE));
    }

    for (int iCntH = 0; iCntH <= GRID_HEIGHT; ++iCntH)
    {
        int iY = (int)((iCntH - g_offsetY) * GRID_SIZE);
        MoveToEx(hdc, (int)((0 - g_offsetX) * GRID_SIZE), iY, NULL);
        LineTo(hdc, (int)((GRID_WIDTH - g_offsetX) * GRID_SIZE), iY);
    }
    SelectObject(hdc, hOldPen);
}

// 부모 표현하기
void RenderParentLine(HDC hdc)
{
    HPEN hOldPen = (HPEN)SelectObject(hdc, g_hParentPen);

    auto pq = openList;
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

void RenderFinalPath(HDC hdc)
{
    if (g_PathEndNode == nullptr)
    {
        return;
    }

    HPEN hOldPen = (HPEN)SelectObject(hdc, g_hPathPen);

    Node* node = g_PathEndNode;
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

void RenderObstacle(HDC hdc)
{
    int iX = 0;
    int iY = 0;
    SelectObject(hdc, GetStockObject(NULL_PEN));

    for (int iCntW = 0; iCntW < GRID_WIDTH; ++iCntW)
    {
        for (int iCntH = 0; iCntH < GRID_HEIGHT; ++iCntH)
        {
            HBRUSH hBrush = nullptr;

            switch (g_Tile[iCntH][iCntW])
            {
            case (char)TILETYPE::Empty:
                hBrush = g_hBrushEmpty;
                break;
            case (char)TILETYPE::Wall:
                hBrush = g_hBrushWall;
                break;
            case (char)TILETYPE::Start:
                hBrush = g_hBrushStart;
                break;
            case (char)TILETYPE::End:
                hBrush = g_hBrushEnd;
                break;
            case (char)TILETYPE::OpenList:
                hBrush = g_hBrushOpenList;
                break;
            case (char)TILETYPE::CloseList:
                hBrush = g_hBrushCloseList;
                break;
            case (char)TILETYPE::FindLoad:
                hBrush = g_hBrushFindLoad;
                break;
            }

            SelectObject(hdc, hBrush);

            iX = (int)((iCntW - g_offsetX) * GRID_SIZE);
            iY = (int)((iCntH - g_offsetY) * GRID_SIZE);
            Rectangle(hdc, iX, iY, iX + GRID_SIZE + 2, iY + GRID_SIZE + 2);
        }
    }
}

Node* JPS_CreateNode(Node* parent, int g, int h, int y, int x, unsigned char dir)
{
    Node* node = JPS_AllocNode(parent, g, h, y, x, dir);
    JPS_CommitNode(node);
    return node;
}

Node* JPS_AllocNode(Node* parent, int g, int h, int y, int x, unsigned char dir)
{
    Node* newNode = new Node;
    newNode->f = g + h;
    newNode->g = g;
    newNode->h = h;
    newNode->y = y;
    newNode->x = x;
    newNode->dir = dir;
    newNode->parent = parent;
    return newNode;
}

bool JPS_CommitNode(Node* node)
{
    openList.push(node);
    if (g_Tile[node->y][node->x] == (char)TILETYPE::Empty)
    {
        g_Tile[node->y][node->x] = (char)TILETYPE::OpenList;
    }
    return true;
}

bool JPS_FindEndNode(Node* node)
{
    while (node != nullptr)
    {
        int x = node->x;
        int y = node->y;

        g_Tile[y][x] = (char)TILETYPE::FindLoad;
        node = node->parent;
    }

    return true;
}

bool JPS_Update(Node* node, int ey, int ex)
{
    int y = node->y;
    int x = node->x;

    // 갔던 길 체크
    if (g_Tile[y][x] == (char)TILETYPE::CloseList)
    {
        return true;
    }

    closeList[{y, x}]++;
    if (y != g_StartY || x != g_StartX)
    {
        g_Tile[y][x] = (char)TILETYPE::CloseList;
    }

    // 목적지인지 체크
    if (y == ey && x == ex)
    {
        g_PathEndNode = node;
        JPS_FindEndNode(node);
        return false;
    }

    // JPS 노드 생성 관련 코드 들어가야함
    if (node->parent == nullptr)
    {
        // Jump 8방향 모두 호출
        // 시작 위치는 호출 노드의 그 다음 한 칸
        Jump_UL(node, node->y - 1, node->x - 1,  ey, ex);
        Jump_UR(node, node->y - 1, node->x + 1, ey, ex);
        Jump_DL(node, node->y + 1, node->x - 1, ey, ex);
        Jump_DR(node, node->y + 1, node->x + 1, ey, ex);
        Jump_UU(node, node->y - 1, node->x, ey, ex);
        Jump_DD(node, node->y + 1, node->x, ey, ex);
        Jump_RR(node, node->y, node->x + 1, ey, ex);
        Jump_LL(node, node->y, node->x - 1, ey, ex);
    }
    else
    {
        // 부모 방향과 같은 Jump 호출
        // 직선
        if (node->dir & (1 << (int)DIRECTION::Jump_RR)) // - 오른쪽 방향
        {
            Jump_RR(node, node->y, node->x + 1, ey, ex);
        }
        if (node->dir & (1 << (int)DIRECTION::Jump_LL)) // - 왼쪽 방향
        {
            Jump_LL(node, node->y, node->x - 1, ey, ex);
        }
        if (node->dir & (1 << (int)DIRECTION::Jump_DD)) // - 아랫 방향
        {
            Jump_DD(node, node->y + 1, node->x, ey, ex);
        }
        if (node->dir & (1 << (int)DIRECTION::Jump_UU)) // - 윗 방향
        {
            Jump_UU(node, node->y - 1, node->x, ey, ex);
        }

        // 대각선
        if (node->dir & (1 << (int)DIRECTION::Jump_UL)) // - 왼쪽 위 대각선 방향
        {
            Jump_UL(node, node->y - 1, node->x - 1, ey, ex);
        }
        if (node->dir & (1 << (int)DIRECTION::Jump_DL)) // - 왼쪽 아래 대각선 방향
        {
            Jump_DL(node, node->y + 1, node->x - 1, ey, ex);
        }
        if (node->dir & (1 << (int)DIRECTION::Jump_UR)) // - 오른쪽 위 대각선 방향
        {
            Jump_UR(node, node->y - 1, node->x + 1, ey, ex);
        }
        if (node->dir & (1 << (int)DIRECTION::Jump_DR)) // - 오른쪽 아래 대각선 방향
        {
            Jump_DR(node, node->y + 1, node->x + 1, ey, ex);
        }
    }

    return true;
}

bool Jump_UL(Node* node, int sy, int sx, int ey, int ex)
{
    int x = sx;
    int y = sy;
    while (true)
    {
        bool flag = false;
        unsigned char dir = (1 << (int)DIRECTION::Jump_UL) | (1 << (int)DIRECTION::Jump_UU) | (1 << (int)DIRECTION::Jump_LL);
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
            dir |= (1 << (int)DIRECTION::Jump_DL);
            flag = true;
        }

        if ((g_Tile[y][x + 1] == (char)TILETYPE::Wall) && (g_Tile[y - 1][x + 1] == (char)TILETYPE::Empty))
        {
            dir |= (1 << (int)DIRECTION::Jump_UR);
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

bool Jump_UR(Node* node, int sy, int sx, int ey, int ex)
{
    int x = sx;
    int y = sy;
    while (true)
    {
        bool flag = false;
        unsigned char dir = (1 << (int)DIRECTION::Jump_UR) | (1 << (int)DIRECTION::Jump_UU) | (1 << (int)DIRECTION::Jump_RR);
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
            dir |= (1 << (int)DIRECTION::Jump_UL);
            flag = true;
        }

        if ((g_Tile[y + 1][x] == (char)TILETYPE::Wall) && (g_Tile[y + 1][x + 1] == (char)TILETYPE::Empty))
        {
            dir |= (1 << (int)DIRECTION::Jump_DR);
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

bool Jump_DL(Node* node, int sy, int sx, int ey, int ex)
{
    int x = sx;
    int y = sy;
    while (true)
    {
        bool flag = false;
        unsigned char dir = (1 << (int)DIRECTION::Jump_DL) | (1 << (int)DIRECTION::Jump_DD) | (1 << (int)DIRECTION::Jump_LL);
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
            dir |= (1 << (int)DIRECTION::Jump_DR);
            flag = true;
        }

        if ((g_Tile[y - 1][x] == (char)TILETYPE::Wall) && (g_Tile[y - 1][x - 1] == (char)TILETYPE::Empty))
        {
            dir |= (1 << (int)DIRECTION::Jump_UL);
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

bool Jump_DR(Node* node, int sy, int sx, int ey, int ex)
{
    int x = sx;
    int y = sy;
    while (true)
    {
        bool flag = false;
        unsigned char dir = (1 << (int)DIRECTION::Jump_DR) | (1 << (int)DIRECTION::Jump_DD) | (1 << (int)DIRECTION::Jump_RR);
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
            dir |= (1 << (int)DIRECTION::Jump_DL);
            flag = true;
        }

        if ((g_Tile[y - 1][x] == (char)TILETYPE::Wall) && (g_Tile[y - 1][x + 1] == (char)TILETYPE::Empty))
        {
            dir |= (1 << (int)DIRECTION::Jump_UR);
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

bool Jump_UU(Node* node, int sy, int sx, int ey, int ex)
{
    int x = sx;
    int y = sy;
    while (true)
    {
        bool flag = false;
        unsigned char dir = (1 << (int)DIRECTION::Jump_UU);
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
            dir |= (1 << (int)DIRECTION::Jump_UR);
            flag = true;
        }

        if (!(sx - 1 < 0) && ((g_Tile[y][sx - 1] == (char)TILETYPE::Wall) &&
            (g_Tile[ny][sx - 1] == (char)TILETYPE::Empty)))
        {
            dir |= (1 << (int)DIRECTION::Jump_UL);
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

bool Jump_DD(Node* node, int sy, int sx, int ey, int ex)
{
    int x = sx;
    int y = sy;
    while (true)
    {
        bool flag = false;
        unsigned char dir = (1 << (int)DIRECTION::Jump_DD);
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
            dir |= (1 << (int)DIRECTION::Jump_DR);
            flag = true;
        }

        if (!(sx - 1 < 0) && ((g_Tile[y][sx - 1] == (char)TILETYPE::Wall) &&
            (g_Tile[ny][sx - 1] == (char)TILETYPE::Empty)))
        {
            dir |= (1 << (int)DIRECTION::Jump_DL);
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

bool Jump_RR(Node* node, int sy, int sx, int ey, int ex)
{
    int x = sx;
    int y = sy;
    while (true)
    {
        bool flag = false;
        unsigned char dir = (1 << (int)DIRECTION::Jump_RR);
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
            dir |= (1 << (int)DIRECTION::Jump_DR);
            flag = true;
        }

        if (!(sy - 1 < 0) && ((g_Tile[sy - 1][x] == (char)TILETYPE::Wall) &&
            (g_Tile[sy - 1][nx] == (char)TILETYPE::Empty)))
        {
            dir |= (1 << (int)DIRECTION::Jump_UR);
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

bool Jump_LL(Node* node, int sy, int sx, int ey, int ex)
{
    int x = sx;
    int y = sy;
    while (true)
    {
        bool flag = false;
        unsigned char dir = (1 << (int)DIRECTION::Jump_LL);
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
            dir |= (1 << (int)DIRECTION::Jump_DL);
            flag = true;
        }

        if (!(sy - 1 < 0) && ((g_Tile[sy - 1][x] == (char)TILETYPE::Wall) &&
            (g_Tile[sy - 1][nx] == (char)TILETYPE::Empty)))
        {
            dir |= (1 << (int)DIRECTION::Jump_UL);
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

bool JPS_Jump_UU_Valid(int sy, int sx, int ey, int ex)
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

bool JPS_Jump_LL_Valid(int sy, int sx, int ey, int ex)
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

bool JPS_Jump_RR_Valid(int sy, int sx, int ey, int ex)
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

bool JPS_Jump_DD_Valid(int sy, int sx, int ey, int ex)
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