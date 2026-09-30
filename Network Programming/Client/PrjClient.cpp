#include "PrjClient.h"
#include "resource.h"

const wchar_t* SERVERIP = L"127.0.0.1";

static HINSTANCE    g_hInstance;
static HWND         g_hBtnSendFile;
static HWND         g_hBtnSendMsg;
static HWND         g_hEditStatus;
static HWND         g_hBtnErasePic;
static HWND         g_hDrawWnd;

static volatile bool        g_isIPv6;
static wchar_t              g_ipaddr[64];
static int                  g_port;
static volatile bool        g_isUDP;
static HANDLE               g_hClientThread;
static volatile bool        g_bCommStarted;
static SOCKET               g_sock;
static HANDLE               g_hReadEvent;
static HANDLE               g_hWriteEvent;

static CHAT_MSG             g_chatmsg;
static DRAWLINE_MSG         g_drawlinemsg;
static int                  g_drawcolor;
static ERASEPIC_MSG         g_erasepicmsg;

long long DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND hChkIsIPv6;
    static HWND hEditIPaddr;
    static HWND hEditPort;
    static HWND hChkIsUDP;
    static HWND hBtnConnect;
    static HWND hBtnSendFile;
    static HWND hBtnSendMsg;
    static HWND hEditMsg;
    static HWND hEditStatus;
    static HWND hColorRed;
    static HWND hColorGreen;
    static HWND hColorBlue;
    static HWND hBtnErasePic;
    static HWND hStaticDummy;

    switch (uMsg)
    {
    case WM_INITDIALOG:
        hChkIsIPv6 = GetDlgItem(hDlg, IDC_ISIPV6);
        hEditIPaddr = GetDlgItem(hDlg, IDC_IPADDR);
        hEditPort = GetDlgItem(hDlg, IDC_PORT);
        hChkIsUDP = GetDlgItem(hDlg, IDC_ISUDP);
        hBtnConnect = GetDlgItem(hDlg, IDC_CONNECT);
        hBtnSendFile = GetDlgItem(hDlg, IDC_SENDFILE);
        g_hBtnSendFile = hBtnSendFile;
        hBtnSendMsg = GetDlgItem(hDlg, IDC_SENDMSG);
        g_hBtnSendMsg = hBtnSendMsg;
        hEditMsg = GetDlgItem(hDlg, IDC_MSG);
        hEditStatus = GetDlgItem(hDlg, IDC_STATUS);
        g_hEditStatus = hEditStatus;
        hColorRed = GetDlgItem(hDlg, IDC_COLORRED);
        hColorGreen = GetDlgItem(hDlg, IDC_COLORGREEN);
        hColorBlue = GetDlgItem(hDlg, IDC_COLORBLUE);
        hBtnErasePic = GetDlgItem(hDlg, IDC_ERASEPIC);
        g_hBtnErasePic = hBtnErasePic;
        hStaticDummy = GetDlgItem(hDlg, IDC_DUMMY);

        SetDlgItemText(hDlg, IDC_IPADDR, SERVERIP);
        SetDlgItemInt(hDlg, IDC_PORT, SERVERPORT, FALSE);
        EnableWindow(g_hBtnSendFile, FALSE);
        EnableWindow(g_hBtnSendMsg, FALSE);
        SendMessage(hEditMsg, EM_SETLIMITTEXT, SIZE_DAT / 2, 0);
        SendMessage(hColorRed, BM_SETCHECK, BST_CHECKED, 0);
        SendMessage(hColorGreen, BM_SETCHECK, BST_UNCHECKED, 0);
        SendMessage(hColorBlue, BM_SETCHECK, BST_UNCHECKED, 0);
        EnableWindow(g_hBtnErasePic, FALSE);

        WNDCLASS wndclass;
        wndclass.style = CS_HREDRAW | CS_VREDRAW;
        wndclass.lpfnWndProc = ChildWndProc;
        wndclass.cbClsExtra = 0;
        wndclass.cbWndExtra = 0;
        wndclass.hInstance = g_hInstance;
        wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        wndclass.lpszMenuName = NULL;
        wndclass.lpszClassName = L"MyWndClass";
        RegisterClass(&wndclass);

        RECT rect;
        GetWindowRect(hStaticDummy, &rect);
        POINT pt;
        pt.x = rect.left;
        pt.y = rect.top;
        ScreenToClient(hDlg, &pt);
        g_hDrawWnd = CreateWindow(L"MyWndClass", L"", WS_CHILD, pt.x, pt.y, rect.right - rect.left,
            rect.bottom - rect.top, hDlg, (HMENU)NULL, g_hInstance, NULL);
        ShowWindow(g_hDrawWnd, SW_SHOW);
        UpdateWindow(g_hDrawWnd);
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_ISIPV6:
            g_isIPv6 = SendMessage(hChkIsIPv6, BM_GETCHECK, 0, 0);
            if (g_isIPv6 == false)
            {
                SetDlgItemText(hDlg, IDC_IPADDR, SERVERIP);
            }
            return TRUE;
        case IDC_CONNECT:
            GetDlgItemText(hDlg, IDC_IPADDR, g_ipaddr, sizeof(g_ipaddr));
            g_port = GetDlgItemInt(hDlg, IDC_PORT, NULL, TRUE);
            g_isIPv6 = SendMessage(hChkIsIPv6, BM_GETCHECK, 0, 0);
            g_isUDP = SendMessage(hChkIsUDP, BM_GETCHECK, 0, 0);
            g_hClientThread = (HANDLE)_beginthreadex(NULL, 0, ClientMain, NULL, 0, NULL);
            while (g_bCommStarted == false);
            EnableWindow(hChkIsIPv6, FALSE);
            EnableWindow(hEditIPaddr, FALSE);
            EnableWindow(hEditPort, FALSE);
            EnableWindow(hChkIsUDP, FALSE);
            EnableWindow(hBtnConnect, FALSE);
            EnableWindow(g_hBtnSendFile, TRUE);
            EnableWindow(g_hBtnSendMsg, TRUE);
            SetFocus(hEditMsg);
            EnableWindow(g_hBtnErasePic, TRUE);
            return TRUE;
        case IDC_SENDFILE:
            MessageBox(NULL, L"아직 구현하지 않았다.", L"알림", MB_ICONERROR);
            return TRUE;
        case IDC_SENDMSG:
            WaitForSingleObject(g_hReadEvent, INFINITE);
            GetDlgItemTextA(hDlg, IDC_MSG, g_chatmsg.msg, SIZE_DAT);
            SetEvent(g_hWriteEvent);
            SendMessage(hEditMsg, EM_SETSEL, 0, -1);
            return TRUE;
        case IDC_COLORRED:
            g_drawlinemsg.color = RGB(255, 0, 0);
            return TRUE;
        case IDC_COLORGREEN:
            g_drawlinemsg.color = RGB(0, 255, 0);
            return TRUE;
        case IDC_COLORBLUE:
            g_drawlinemsg.color = RGB(0, 0, 255);
            return TRUE;
        case IDC_ERASEPIC:
            send(g_sock, (char*)&g_erasepicmsg, SIZE_DAT, 0);
            return TRUE;
        case IDCANCEL:
            closesocket(g_sock);
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
        }
    }

    return FALSE;
}

long long ChildWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    HPEN hPen, hOldPen;
    PAINTSTRUCT ps;
    static int cx, cy;
    static HBITMAP hBitmap;
    static HDC hDCMem;
    static int sy, sx;
    static int ey, ex;
    static bool bDrawing;

    switch (uMsg)
    {
    case WM_SIZE:
        hdc = GetDC(hWnd);
        cx = LOWORD(lParam);
        cy = HIWORD(lParam);
        hBitmap = CreateCompatibleBitmap(hdc, cx, cy);
        hDCMem = CreateCompatibleDC(hdc);
        SelectObject(hDCMem, hBitmap);
        SelectObject(hDCMem, GetStockObject(WHITE_BRUSH));
        SelectObject(hDCMem, GetStockObject(WHITE_PEN));
        Rectangle(hDCMem, 0, 0, cx, cy);
        ReleaseDC(hWnd, hdc);
        return 0;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        BitBlt(hdc, 0, 0, cx, cy, hDCMem, 0, 0, SRCCOPY);
        EndPaint(hWnd, &ps);
        return 0;
    case WM_LBUTTONDOWN:
        sx = LOWORD(lParam);
        sy = HIWORD(lParam);
        bDrawing = true;
        return 0;
    case WM_MOUSEMOVE:
        if (bDrawing && g_bCommStarted)
        {
            ex = LOWORD(lParam);
            ey = HIWORD(lParam);
            g_drawlinemsg.sx = sx;
            g_drawlinemsg.sy = sy;
            g_drawlinemsg.ex = ex;
            g_drawlinemsg.ey = ey;
            send(g_sock, (char*)&g_drawlinemsg, SIZE_TOT, 0);
            sy = ey;
            sx = ex;
        }
        return 0;
    case WM_LBUTTONUP:
        bDrawing = false;
        return 0;
    case WM_DRAWLINE:
        hdc = GetDC(hWnd);
        hPen = CreatePen(PS_SOLID, 3, g_drawcolor);
        hOldPen = (HPEN)SelectObject(hdc, hPen);
        MoveToEx(hdc, LOWORD(wParam), HIWORD(wParam), NULL);
        LineTo(hdc, LOWORD(lParam), HIWORD(lParam));
        SelectObject(hdc, hOldPen);

        hOldPen = (HPEN)SelectObject(hDCMem, hPen);
        MoveToEx(hDCMem, LOWORD(wParam), HIWORD(wParam), NULL);
        LineTo(hDCMem, LOWORD(lParam), HIWORD(lParam));
        SelectObject(hDCMem, hOldPen);
        DeleteObject(hPen);
        ReleaseDC(hWnd, hdc);
        return 0;
    case WM_ERASEPIC:
        SelectObject(hDCMem, GetStockObject(WHITE_BRUSH));
        SelectObject(hDCMem, GetStockObject(WHITE_PEN));
        Rectangle(hDCMem, 0, 0, cx, cy);
        InvalidateRect(hWnd, NULL, FALSE);
        return 0;
    case WM_DESTROY:
        DeleteDC(hDCMem);
        DeleteObject(hBitmap);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

unsigned int __stdcall ClientMain(LPVOID arg)
{
    if (g_isIPv6 == false && g_isUDP == false)
    {
        g_sock = socket(AF_INET, SOCK_STREAM, 0);

        SOCKADDR_IN serveraddr;
        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        InetPton(AF_INET, g_ipaddr, &serveraddr.sin_addr);
        serveraddr.sin_port = htons(g_port);
        auto connectRet = connect(g_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    }

    MessageBox(NULL, L"서버에 접속했습니다.", L"알림", MB_ICONINFORMATION);

    HANDLE hThread[2];
    hThread[0] = (HANDLE)_beginthreadex(NULL, 0, ReadThread, NULL, 0, NULL);
    hThread[0] = (HANDLE)_beginthreadex(NULL, 0, WriteThread, NULL, 0, NULL);
    g_bCommStarted = true;

    auto wfmoRet = WaitForMultipleObjects(2, hThread, FALSE, INFINITE);
    CloseHandle(hThread[0]);
    CloseHandle(hThread[1]);

    MessageBox(NULL, L"연결이 종료되었습니다.", L"알림", MB_ICONERROR);

    EnableWindow(g_hBtnSendFile, FALSE);
    EnableWindow(g_hBtnSendMsg, FALSE);
    EnableWindow(g_hBtnErasePic, FALSE);
    g_bCommStarted = false;
    closesocket(g_sock);

    return 0;
}

unsigned int __stdcall ReadThread(LPVOID arg)
{
    COMM_MSG comm_msg;
    CHAT_MSG* chat_msg;
    DRAWLINE_MSG* drawline_msg;
    ERASEPIC_MSG* erasepic_msg;

    while (true)
    {
        auto recvRet = recv(g_sock, (char*)&comm_msg, SIZE_TOT, MSG_WAITALL);
        if (recvRet == 0 || recvRet == SOCKET_ERROR)
        {
            break;
        }

        if (comm_msg.type == TYPE_CHAT)
        {
            chat_msg = (CHAT_MSG*)&comm_msg;
            DisplayText("[받은 메시지] %s\r\n", chat_msg->msg);
        }
        else if (comm_msg.type == TYPE_DRAWLINE)
        {
            drawline_msg = (DRAWLINE_MSG*)&comm_msg;
            g_drawcolor = drawline_msg->color;
            SendMessage(g_hDrawWnd, WM_DRAWLINE, MAKEWPARAM(drawline_msg->sx, drawline_msg->sy),
                MAKELPARAM(drawline_msg->ex, drawline_msg->ey));
        }
        else if (comm_msg.type == TYPE_ERASEPIC)
        {
            erasepic_msg = (ERASEPIC_MSG*)&comm_msg;
            SendMessage(g_hDrawWnd, WM_ERASEPIC, 0, 0);
        }
    }

    return 0;
}

unsigned int __stdcall WriteThread(LPVOID arg)
{
    while (true)
    {
        WaitForSingleObject(g_hWriteEvent, INFINITE);

        if (strlen(g_chatmsg.msg) == 0)
        {
            EnableWindow(g_hBtnSendMsg, TRUE);
            SetEvent(g_hReadEvent);
            continue;
        }

        auto sendRet = send(g_sock, (char*)&g_chatmsg, SIZE_TOT, 0);
        if (sendRet == SOCKET_ERROR)
        {
            break;
        }

        EnableWindow(g_hBtnSendMsg, TRUE);

        SetEvent(g_hReadEvent);
    }

    return 0;
}

void DisplayText(const char* fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    char cbuf[1024];
    wvsprintfA(cbuf, fmt, arg);
    va_end(arg);

    int nLength = GetWindowTextLength(g_hEditStatus);
    SendMessage(g_hEditStatus, EM_SETSEL, nLength, nLength);
    SendMessageA(g_hEditStatus, EM_REPLACESEL, FALSE, (LPARAM)cbuf);
}