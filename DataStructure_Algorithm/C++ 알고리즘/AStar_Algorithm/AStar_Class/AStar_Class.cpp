// AStar_Class.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "AStar.h"
#include "AStar_Class.h"
#include <random>
#include <queue>
#include <utility>
#include <chrono>

#pragma comment(lib, "msimg32.lib")

#define MAX_LOADSTRING 100

/* 검증 코드 */
HWND g_hWnd = nullptr;
bool g_bAutoTest = false;
uint64_t g_AutoTestTimerId = 1;
int g_TestCount = 0;
int g_MismatchCount = 0;
//======================================

int g_Best[GRID_HEIGHT][GRID_WIDTH];
AStar* g_Astar = nullptr;
bool g_isrun = false;

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
void RenderObstacle(HDC hdc);
void RenderHelpText(HDC hdc);

/* 검증 코드 */
void GenerateRandomMap(double wall = 0.3);
bool IsPathReachable(int sy, int sx, int ey, int ex);
void SeedNewRandomTest();
//=============================
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    g_Astar = AStar::getInstance();
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
    LoadStringW(hInstance, IDC_ASTARCLASS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ASTARCLASS));

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

    return (int)msg.wParam;
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

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_ASTARCLASS));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_ASTARCLASS);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

    // 클라이언트 영역이 GRID_WIDTH x GRID_HEIGHT(GRID_SIZE 픽셀 단위) 전체를 담도록 계산
    RECT rc = { 0, 0, GRID_WIDTH * GRID_SIZE, GRID_HEIGHT * GRID_SIZE };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, TRUE);

    int windowWidth = rc.right - rc.left;
    int windowHeight = rc.bottom - rc.top;

    // 모니터보다 창이 커지지 않도록 안전하게 클램프
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    if (windowWidth > screenWidth) windowWidth = screenWidth;
    if (windowHeight > screenHeight) windowHeight = screenHeight;

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    g_hWnd = hWnd;

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
                g_Astar->AS_Clear();
                int g = 0;
                int h = (abs(g_EndY - g_StartY) + abs(g_EndX - g_StartX)) * DISTANCE;
                g_Astar->AS_CreateNode(nullptr, g, h, g_StartY, g_StartX);
                g_bStart = true;
            }

            if (g_StartX != -1 && g_StartY != -1 && g_EndX != -1 && g_EndY != -1)
            {
                // openList, closeList 그리드 표현
                if (g_Astar->AS_Run(g_StartY, g_StartX, g_EndY, g_EndX))
                {
                    if (g_Astar->isError == true)
                    {
                        printf("FindLoad Fail!!\n");
                        __debugbreak();
                    }

                    InvalidateRect(hWnd, NULL, true);
                }
            }
        }
        else if (wParam == VK_TAB)  // 한 번에 진행
        {
            if (!g_bStart)
            {
                g_Astar->AS_Clear();
                int g = 0;
                int h = (abs(g_EndY - g_StartY) + abs(g_EndX - g_StartX)) * DISTANCE;
                g_Astar->AS_CreateNode(nullptr, g, h, g_StartY, g_StartX);
                g_bStart = true;
            }

            if (g_StartX != -1 && g_StartY != -1 && g_EndX != -1 && g_EndY != -1)
            {
                while (g_Astar->AS_Run(g_StartY, g_StartX, g_EndY, g_EndX))
                {
                    if (g_Astar->isError == true)
                    {
                        printf("FindLoad Fail!!\n");
                        __debugbreak();
                    }

                    if (g_Astar->isFindLoad == true)
                    {
                        break;
                    }

                    InvalidateRect(hWnd, NULL, true);
                }
            }
        }
        else if (wParam == 'R')
        {
            SeedNewRandomTest();
            InvalidateRect(hWnd, NULL, true);
        }
        else if (wParam == 'A')
        {
            g_bAutoTest = !g_bAutoTest;
            if (g_bAutoTest)
            {
                SetTimer(hWnd, g_AutoTestTimerId, 1000, NULL);
                printf("====== Test Start ======\n");
            }
            else
            {
                KillTimer(hWnd, g_AutoTestTimerId);
                printf("====== Test Stop (%d of %d fail) ======\n", g_TestCount, g_MismatchCount);
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
    case WM_TIMER:
        if (wParam == g_AutoTestTimerId)
        {
            auto start = std::chrono::steady_clock::now();

            GenerateRandomMap(0.3);
            bool expectedReachable = IsPathReachable(g_StartY, g_StartX, g_EndY, g_EndX);

            g_Astar->AS_Clear();
            g_Astar->isError = false;
            g_Astar->isFindLoad = false;
            int h = (abs(g_EndY - g_StartY) + abs(g_EndX - g_StartX)) * DISTANCE;
            g_Astar->AS_CreateNode(nullptr, 0, h, g_StartY, g_StartX);

            while (g_Astar->AS_Run(g_StartY, g_StartX, g_EndY, g_EndX))
            {
                if (g_Astar->isError || g_Astar->isFindLoad)
                {
                    break;
                }
            }

            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::milli> dur = end - start;

            bool actualFound = g_Astar->isFindLoad;
            ++g_TestCount;

            printf("[#%d] expected=%d actual=%d (start:%d,%d end:%d,%d) (duration:%lf)\n",
                g_TestCount, expectedReachable, actualFound,
                g_StartX, g_StartY, g_EndX, g_EndY, dur.count());

            InvalidateRect(hWnd, NULL, true);

            if (expectedReachable != actualFound)
            {
                ++g_MismatchCount;
                KillTimer(hWnd, g_AutoTestTimerId);
                g_bAutoTest = false;
                __debugbreak();
            }
            else
            {
                printf("Find Load!!!\n");
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
        g_Astar->AS_RenderParentLine(hdc);
        g_Astar->AS_RenderFinalPath(hdc);
        RenderHelpText(hdc);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        KillTimer(hWnd, g_AutoTestTimerId);
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
        g_Astar->destoryInstance();
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

void RenderHelpText(HDC hdc)
{
    static const WCHAR* lines[] = {
            L"[조작법]",
            L"좌클릭 : 출발지 / 목적지 지정",
            L"우클릭 드래그 : 벽 생성 / 삭제",
            L"마우스 휠 : 확대 / 축소",
            L"SPACE : 한 스텝 진행",
            L"TAB : 끝까지 한 번에 진행",
            L"R : 새 랜덤 맵 생성",
            L"A : 자동 검증 모드 토글",
    };
    const int lineCount = sizeof(lines) / sizeof(lines[0]);

    const int padding = 8;
    const int lineHeight = 18;
    const int boxWidth = 260;
    const int boxHeight = padding * 2 + lineHeight * lineCount;

    // 1. 반투명 패널용 메모리 DC/비트맵 생성
    HDC hMemDC = CreateCompatibleDC(hdc);
    HBITMAP hMemBmp = CreateCompatibleBitmap(hdc, boxWidth, boxHeight);
    HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hMemBmp);

    RECT rc = { 0, 0, boxWidth, boxHeight };
    HBRUSH hBrush = CreateSolidBrush(RGB(30, 30, 30));
    FillRect(hMemDC, &rc, hBrush);
    DeleteObject(hBrush);

    // 2. 알파값 지정해서 원본 hdc(=맵이 이미 그려진) 위에 블렌딩
    BLENDFUNCTION blend = {};
    blend.BlendOp = AC_SRC_OVER;
    blend.BlendFlags = 0;
    blend.SourceConstantAlpha = 160;
    blend.AlphaFormat = 0;

    AlphaBlend(hdc, 0, 0, boxWidth, boxHeight,
        hMemDC, 0, 0, boxWidth, boxHeight, blend);

    SelectObject(hMemDC, hOldBmp);
    DeleteObject(hMemBmp);
    DeleteDC(hMemDC);

    // 3. 테두리 + 텍스트는 hdc에 직접, 또렷하게
    FrameRect(hdc, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255));

    for (int i = 0; i < lineCount; ++i)
    {
        TextOutW(hdc, padding, padding + i * lineHeight, lines[i], (int)wcslen(lines[i]));
    }
}

void GenerateRandomMap(double wall)
{
    static std::mt19937 rng(std::random_device {}());
    std::uniform_int_distribution<int> distX(0, GRID_WIDTH - 1);
    std::uniform_int_distribution<int> distY(0, GRID_HEIGHT - 1);
    std::uniform_real_distribution<double> distWall(0.0, 1.0);

    memset(g_Tile, (char)TILETYPE::Empty, sizeof(g_Tile));
    for (auto i = 0; i < GRID_HEIGHT; ++i)
    {
        for (auto j = 0; j < GRID_WIDTH; ++j)
        {
            g_Best[i][j] = INT_MAX;
        }
    }

    for (auto y = 0; y < GRID_HEIGHT; ++y)
    {
        for (auto x = 0; x < GRID_WIDTH; ++x)
        {
            if (distWall(rng) < wall)
            {
                g_Tile[y][x] = (char)TILETYPE::Wall;
            }
        }
    }

    do
    {
        g_StartY = distY(rng);
        g_StartX = distX(rng);
    } while (g_Tile[g_StartY][g_StartX] == (char)TILETYPE::Wall);

    do
    {
        g_EndY = distY(rng);
        g_EndX = distX(rng);
    } while (g_Tile[g_EndY][g_EndX] == (char)TILETYPE::Wall
        || (g_EndX == g_StartX && g_EndY == g_StartY));

    g_Tile[g_StartY][g_StartX] = (char)TILETYPE::Start;
    g_Tile[g_EndY][g_EndX] = (char)TILETYPE::End;
}

bool IsPathReachable(int sy, int sx, int ey, int ex)
{
    static const int dy[8] = { -1, 0, 1, 0, -1, 1, 1, -1 };
    static const int dx[8] = { 0, 1, 0, -1, 1, 1, -1, -1 };

    static bool visited[GRID_HEIGHT][GRID_WIDTH];
    memset(visited, 0, sizeof(visited));

    std::queue<std::pair<int, int>> q;
    q.push({ sy, sx });
    visited[sy][sx] = true;

    while (!q.empty())
    {
        auto [cy, cx] = q.front();
        q.pop();

        if (cy == ey && cx == ex)
            return true;

        for (int k = 0; k < 8; ++k)
        {
            int ny = cy + dy[k];
            int nx = cx + dx[k];
            if (ny < 0 || ny >= GRID_HEIGHT || nx < 0 || nx >= GRID_WIDTH)
                continue;
            if (visited[ny][nx] || g_Tile[ny][nx] == (char)TILETYPE::Wall)
                continue;

            visited[ny][nx] = true;
            q.push({ ny, nx });
        }
    }
    return false;
}

void SeedNewRandomTest()
{
    GenerateRandomMap(0.3);

    g_Astar->AS_Clear();
    g_Astar->isError = false;
    g_Astar->isFindLoad = false;

    int h = (abs(g_EndY - g_StartY) + abs(g_EndX - g_StartX)) * DISTANCE;
    g_Astar->AS_CreateNode(nullptr, 0, h, g_StartY, g_StartX);
    g_bStart = true;
}