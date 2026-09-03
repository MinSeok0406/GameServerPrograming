// CreateFindRoad_Tool.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "CreateFindRoad_Tool.h"
//#include "AStar.h"
#include <vector>
#include <map>
#include <queue>

#define MAX_LOADSTRING 100

// AStar Algorithm
//---------------------------
#define GRID_WIDTH 100
#define GRID_HEIGHT 50
#define DISTANCE 10
#define DIGSTANCE 14

// g -> 유클리드
// h -> 맨해튼
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


const int dy[8] = { -1, 0, 1, 0, -1, 1, 1, -1 };
const int dx[8] = { 0, 1, 0, -1, 1, 1, -1, -1 };
int g_Best[GRID_HEIGHT][GRID_WIDTH];
//AStar* g_Astar = nullptr;
std::priority_queue<Node*, std::vector<Node*>, Comp> openList;
std::map<std::pair<int, int>, int> closeList;
Node* g_PathEndNode = nullptr;
bool g_isrun = false;

bool AS_CreateNode(Node* parent, int g, int h, int y, int x);
bool AS_FindEndNode(Node* node);
bool AS_Update(Node* node, int ey, int ex);
//---------------------------

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

//-----------------------

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

//------------------------
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

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

bool ScreenToTile(int xPos, int yPos, int* outTileX, int* outTileY);
void RenderGrid(HDC hdc);
void RenderParentLine(HDC hdc);
void RenderFinalPath(HDC hdc);
void RenderObstacle(HDC hdc);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
    AllocConsole(); // 콘솔창 생성
    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CREATEFINDROADTOOL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CREATEFINDROADTOOL));

    for (auto i = 0; i < GRID_HEIGHT; ++i)
    {
        for (auto j = 0; j < GRID_WIDTH; ++j)
        {
            g_Best[i][j] = INT_MAX;
        }
    }

    //g_Astar = AStar::getInstance();

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CREATEFINDROADTOOL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CREATEFINDROADTOOL);
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
    PAINTSTRUCT ps;
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
                int g = 0;
                int h = (abs(g_EndY - g_StartY) + abs(g_EndX - g_StartX)) * DISTANCE;
                AS_CreateNode(nullptr, g, h, g_StartY, g_StartX);
                g_bStart = true;
            }

            if (g_StartX != -1 && g_StartY != -1 && g_EndX != -1 && g_EndY != -1)
            {
                // openList, closeList 그리드 표현
                if (openList.empty() == false)
                {
                    Node* node = openList.top();
                    openList.pop();
                    AS_Update(node, g_EndY, g_EndX);
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
                int g = 0;
                int h = (abs(g_EndY - g_StartY) + abs(g_EndX - g_StartX)) * DISTANCE;
                AS_CreateNode(nullptr, g, h, g_StartY, g_StartX);
                g_bStart = true;
            }

            if (g_StartX != -1 && g_StartY != -1 && g_EndX != -1 && g_EndY != -1)
            {
                while (openList.empty() == false)
                {
                    Node* node = openList.top();
                    openList.pop();
                    if (!AS_Update(node, g_EndY, g_EndX))
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

    if (g_Tile[y][x] == (char)TILETYPE::Empty)
    {
        g_Tile[y][x] = (char)TILETYPE::OpenList;
    }
    
    return true;
}

bool AS_FindEndNode(Node* node)
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

bool AS_Update(Node* node, int ey, int ex)
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
        AS_FindEndNode(node);
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

        // 노드가 이미 존재한다면 스킵
        /*if (g_Tile[ny][nx] == (char)TILETYPE::OpenList)
        {
            continue;
        }*/

        // 직각 거리 & 대각선 거리 계산
        // 노드 생성해서 넣기
        // G : 유클리드, H : 맨해튼
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
