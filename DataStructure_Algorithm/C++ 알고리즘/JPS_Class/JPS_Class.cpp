// JPS_Class.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "JPS_Class.h"
#include "JPS.h"
#include <random>
#include <queue>
#include <utility>
#include <chrono>

#define MAX_LOADSTRING 100

#define GRID_WIDTH 100
#define GRID_HEIGHT 50

/* 검증 코드 */
HWND g_hWnd = nullptr;
bool g_bAutoTest = false;
uint64_t g_AutoTestTimerId = 1;
int g_TestCount = 0;
int g_MismatchCount = 0;

//=============================

JPS* g_JPS = nullptr;
const int g_mapID = 0;
bool g_isrun = false;

HPEN g_hGridPen;
HPEN g_hParentPen;
HPEN g_hPathPen;
HPEN g_hBresenhamPen;
HBRUSH g_hBrushEmpty;
HBRUSH g_hBrushWall;
HBRUSH g_hBrushStart;
HBRUSH g_hBrushEnd;
HBRUSH g_hBrushOpenList;
HBRUSH g_hBrushCloseList;
HBRUSH g_hBrushFindLoad;
HFONT g_hHelpFont;

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
void RenderObstacle(HDC hdc);
void RenderHelpText(HDC hdc);

/* 검증 코드 */
void GenerateRandomMap(double wall = 0.3);
bool IsPathReachable(int sy, int sx, int ey, int ex);
void SeedNewRandomTest();

// 프로파일링
//=====================
struct MapData
{
    char map[GRID_HEIGHT][GRID_WIDTH];
};

bool cmp(std::pair<double, MapData>& a, std::pair<double, MapData>& b)
{
    return a.first > b.first;
}

std::vector<std::pair<double, MapData>> v;

double g_maxDuration = -1.0;
double g_minDuration = 123456789.0;
//=====================

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
    g_JPS = JPS::getInstance();
    g_JPS->JPS_Init(g_mapID, GRID_HEIGHT, GRID_WIDTH);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    AllocConsole(); // 콘솔창 생성

    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout); // stdout을 방금 만든 콘솔로 연결
    freopen_s(&fp, "CONOUT$", "w", stderr); // stderr도 연결
    freopen_s(&fp, "CONIN$", "r", stdin);  // 필요하면 입력도 연결

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_JPSCLASS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_JPSCLASS));

/*    FILE* file = nullptr;
    fopen_s(&file, "JPS_MapData.txt", "rb");

    fread(g_Tile, sizeof(char), GRID_HEIGHT * GRID_WIDTH, file);
    fclose(file);*/

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_JPSCLASS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_JPSCLASS);
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
                g_JPS->JPS_Clear();
                unsigned char dir = 0;
                for (auto i = 0; i < 8; ++i)
                {
                    dir |= (1 << i);
                }
                int g = 0;
                int h = (abs(g_EndY - g_StartY) + abs(g_EndX - g_StartX)) * DISTANCE;
                g_JPS->JPS_CreateNode(g_mapID, nullptr, g, h, g_StartY, g_StartX, dir);
                g_bStart = true;
            }

            if (g_StartX != -1 && g_StartY != -1 && g_EndX != -1 && g_EndY != -1)
            {
                // openList, closeList 그리드 표현
                if (g_JPS->JPS_Run(g_mapID, g_StartY, g_StartX, g_EndY, g_EndX))
                {
                    if (g_JPS->isError == true)
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
                g_JPS->JPS_Clear();
                unsigned char dir = 0;
                for (auto i = 0; i < 8; ++i)
                {
                    dir |= (1 << i);
                }
                int g = 0;
                int h = (abs(g_EndY - g_StartY) + abs(g_EndX - g_StartX)) * DISTANCE;
                g_JPS->JPS_CreateNode(g_mapID, nullptr, g, h, g_StartY, g_StartX, dir);
                g_bStart = true;
            }

            if (g_StartX != -1 && g_StartY != -1 && g_EndX != -1 && g_EndY != -1)
            {
                auto start = std::chrono::steady_clock::now();

                while (g_JPS->JPS_Run(g_mapID, g_StartY, g_StartX, g_EndY, g_EndX))
                {
                    if (g_JPS->isError == true)
                    {
                        printf("FindLoad Fail!!\n");
                        __debugbreak();
                    }

                    if (g_JPS->isFindLoad == true)
                    {
                        break;
                    }

                    InvalidateRect(hWnd, NULL, true);
                }

                auto end = std::chrono::steady_clock::now();
                std::chrono::duration<double, std::milli> dur = end - start;

                printf("(start:%d,%d end:%d,%d) (duration:%lf)\n", g_StartX, g_StartY, g_EndX, g_EndY, dur.count());
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
                SetTimer(hWnd, g_AutoTestTimerId, 100, NULL);
                printf("====== Test Start ======\n");
            }
            else
            {
                KillTimer(hWnd, g_AutoTestTimerId);
                printf("====== Test Stop (%d of %d fail) ======\n", g_TestCount, g_MismatchCount);
            }
        }
        else if (wParam == 'P')
        {
            // 횟수, 최대, 최소 시간 저장
            FILE* file = nullptr;
            fopen_s(&file, "JPS_Profiling.txt", "w");
            if (file == NULL)
            {
                __debugbreak();
            }

            fprintf(file, "TestCount = %d\n", g_TestCount);
            fprintf(file, "min Duration = %.6lf ms\n", g_minDuration);
            fprintf(file, "max Duration = %.6lf ms\n", g_maxDuration);
            fclose(file);

            printf("JPS_Profiling.txt 저장 완료 (max=%.6lf ms, min=%.6lf ms, count=%d)\n",
                g_maxDuration, g_minDuration, g_TestCount);


            // 맵 데이터 저장
            sort(v.begin(), v.end(), cmp);
            FILE* fp = nullptr;
            fopen_s(&fp, "JPS_MapData.txt", "wb");
            if (fp == NULL)
            {
                __debugbreak();
            }

            MapData& md = v[0].second;
            fwrite(md.map, sizeof(char), GRID_HEIGHT * GRID_WIDTH, fp);
            fclose(fp);
        }
        else if (wParam == 'B')
        {
            g_bStartDrag = false;
            g_bEndDrag = !g_bEndDrag;
            g_JPS->JPS_Init(g_mapID, GRID_HEIGHT, GRID_WIDTH);

            for (auto i = 0; i < GRID_HEIGHT; ++i)
            {
                for (auto j = 0; j < GRID_WIDTH; ++j)
                {
                    if (g_Tile[i][j] == (char)TILETYPE::Start)
                    {
                        g_StartY = i;
                        g_StartX = j;
                        g_JPS->JPS_TileStartEnd(g_mapID, i, j, false);
                    }
                    else if (g_Tile[i][j] == (char)TILETYPE::End)
                    {
                        g_EndY = i;
                        g_EndX = j;
                        g_JPS->JPS_TileStartEnd(g_mapID, i, j, true);
                    }
                    else if (g_Tile[i][j] == (char)TILETYPE::Wall)
                    {
                        g_JPS->JPS_TileWall(g_mapID, i, j, false);
                    }
                    else if (g_Tile[i][j] == (char)TILETYPE::Empty)
                    {
                        g_JPS->JPS_TileWall(g_mapID, i, j, true);
                    }
                }
            }

            g_JPS->JPS_Clear();
            unsigned char dir = 0;
            for (auto i = 0; i < 8; ++i)
            {
                dir |= (1 << i);
            }
            int g = 0;
            int h = (abs(g_EndY - g_StartY) + abs(g_EndX - g_StartX)) * DISTANCE;
            g_JPS->JPS_CreateNode(g_mapID, nullptr, g, h, g_StartY, g_StartX, dir);

            InvalidateRect(hWnd, NULL, true);
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
                    g_JPS->JPS_TileStartEnd(g_mapID, iTileY, iTileX, g_bEndDrag);
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
                    g_JPS->JPS_TileStartEnd(g_mapID, iTileY, iTileX, g_bEndDrag);
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
                g_JPS->JPS_TileWall(g_mapID, iTileY, iTileX, g_bWallErase);
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

            g_JPS->JPS_Clear();
            g_JPS->isError = false;
            g_JPS->isFindLoad = false;
            int h = (abs(g_EndY - g_StartY) + abs(g_EndX - g_StartX)) * DISTANCE;

            unsigned char dir = 0;
            for (auto i = 0; i < 8; ++i)
            {
                dir |= (1 << i);
            }
            g_JPS->JPS_CreateNode(g_mapID, nullptr, 0, h, g_StartY, g_StartX, dir);

            MapData md;
            memcpy(md.map, g_Tile, sizeof(g_Tile));

            while (g_JPS->JPS_Run(g_mapID, g_StartY, g_StartX, g_EndY, g_EndX))
            {
                if (g_JPS->isError || g_JPS->isFindLoad)
                {
                    break;
                }
            }

            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::milli> dur = end - start;

            bool actualFound = g_JPS->isFindLoad;
            ++g_TestCount;

            printf("[#%d] expected=%d actual=%d (start:%d,%d end:%d,%d) (duration:%lf)\n",
                g_TestCount, expectedReachable, actualFound,
                g_StartX, g_StartY, g_EndX, g_EndY, dur.count());

            g_maxDuration = max(g_maxDuration, dur.count());
            g_minDuration = min(g_minDuration, dur.count());
            v.push_back({ g_maxDuration, md });

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
        g_hParentPen = CreatePen(PS_SOLID, 3, RGB(150, 150, 200));
        g_hPathPen = CreatePen(PS_SOLID, 4, RGB(255, 150, 200));
        g_hBresenhamPen = CreatePen(PS_SOLID, 3, RGB(100, 100, 200));
        g_hBrushEmpty = CreateSolidBrush(RGB(255, 255, 255));
        g_hBrushWall = CreateSolidBrush(RGB(100, 100, 100));
        g_hBrushStart = CreateSolidBrush(RGB(0, 200, 0));
        g_hBrushEnd = CreateSolidBrush(RGB(200, 0, 0));
        g_hBrushOpenList = CreateSolidBrush(RGB(0, 0, 200));
        g_hBrushCloseList = CreateSolidBrush(RGB(255, 255, 0));
        g_hBrushFindLoad = CreateSolidBrush(RGB(255, 0, 255));
        g_hHelpFont = CreateFontA(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,   // 추가
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Consolas");
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps);
        RenderObstacle(hdc);
        RenderGrid(hdc);
        g_JPS->JPS_RenderParentLine(hdc);
        g_JPS->JPS_RenderFinalPath(hdc);
        g_JPS->JPS_RenderBresenhamLine(g_mapID ,hdc);
        RenderHelpText(hdc);
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
        DeleteObject(g_hHelpFont);
        DeleteObject(g_hBresenhamPen);
        g_JPS->destroyInstance();
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
    const char* helpLines[] = {
        "[Space] 1 Step",
        "[Tab] Run All",
        "[R] New Random Map",
        "[A] Auto Test Toggle",
        "[P] Save Profiling(.txt)",
        "[LClick] Set Start/End",
        "[RClick+Drag] Wall Toggle/Erase",
        "[Wheel] Zoom In/Out",
    };
    int lineCount = sizeof(helpLines) / sizeof(helpLines[0]);

    HFONT hOldFont = (HFONT)SelectObject(hdc, g_hHelpFont);
    int oldBkMode = SetBkMode(hdc, TRANSPARENT);
    COLORREF oldColor = SetTextColor(hdc, RGB(20, 20, 20));

    int y = 5;
    for (int i = 0; i < lineCount; ++i)
    {
        TextOutA(hdc, 5, y, helpLines[i], (int)strlen(helpLines[i]));
        y += 18;
    }

    SetTextColor(hdc, oldColor);
    SetBkMode(hdc, oldBkMode);
    SelectObject(hdc, hOldFont);
}

void GenerateRandomMap(double wall)
{
    //static std::mt19937 rng(3246);
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> distX(0, GRID_WIDTH - 1);
    std::uniform_int_distribution<int> distY(0, GRID_HEIGHT - 1);
    std::uniform_real_distribution<double> distWall(0.0, 1.0);

    memset(g_Tile, (char)TILETYPE::Empty, sizeof(g_Tile));
    g_JPS->JPS_Init(g_mapID, GRID_HEIGHT, GRID_WIDTH);

    for (auto y = 0; y < GRID_HEIGHT; ++y)
    {
        for (auto x = 0; x < GRID_WIDTH; ++x)
        {
            if (distWall(rng) < wall)
            {
                g_Tile[y][x] = (char)TILETYPE::Wall;
                g_JPS->JPS_TileWall(g_mapID, y, x, false);
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
    g_JPS->JPS_TileStartEnd(g_mapID, g_StartY, g_StartX, false);
    g_JPS->JPS_TileStartEnd(g_mapID, g_EndY, g_EndX, true);
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
        int cy, cx;
        std::tie(cy, cx) = q.front();
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

    g_JPS->JPS_Clear();
    g_JPS->isError = false;
    g_JPS->isFindLoad = false;

    unsigned char dir = 0;
    for (auto i = 0; i < 8; ++i)
    {
        dir |= (1 << i);
    }
    int h = (abs(g_EndY - g_StartY) + abs(g_EndX - g_StartX)) * DISTANCE;
    g_JPS->JPS_CreateNode(g_mapID, nullptr, 0, h, g_StartY, g_StartX, dir);
    g_bStart = true;
}