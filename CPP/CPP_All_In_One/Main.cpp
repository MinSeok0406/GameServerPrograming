#include <iostream>
#include <tchar.h>
#include <queue>
#include <stack>
#include <fcntl.h>
#include <io.h>
#include <conio.h>
#include <chrono>
#include <strsafe.h>
#include <WinSock2.h>
#include <chrono>
#include "Temp.h"
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")

#ifdef UNICODE
#define tcout std::wcout
#define tcin std::wcin
#define tcerr std::wcerr
#define tclog std::wclog
#else
#define tcout std::cout
#define tcin std::cin
#define tcerr std::cerr
#define tclog std::clog
#endif

#ifdef _UNICODE
#define tcout std::wcout
#define tcin std::wcin
#define tcerr std::wcerr
#define tclog std::wclog
#else
#define tcout std::cout
#define tcin std::cin
#define tcerr std::cerr
#define tclog std::clog
#endif

#define BUF_SIZE 1024
int CommToClient(HANDLE);

int _tmain(int argc, TCHAR* argv[])
{
    timeBeginPeriod(1);
    system("chcp 65001");
    
    LPCTSTR pipeName = _T("\\\\.\\pipe\\simple_pipe");
    HANDLE hPipe;
    while (1)
    {
        hPipe = CreateNamedPipe(pipeName, PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES, BUF_SIZE, BUF_SIZE, 20000, NULL);

        if (hPipe == INVALID_HANDLE_VALUE)
        {
            return -1;
        }

        BOOL isSuccess = 0;
        isSuccess = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

        if (isSuccess)
        {
            CommToClient(hPipe);
        }
        else
        {
            CloseHandle(hPipe);
        }
    }
    
    return 1;
}

int CommToClient(HANDLE hPipe)
{
    TCHAR fileName[MAX_PATH];
    TCHAR dataBuf[BUF_SIZE];

    BOOL isSuccess;
    DWORD fileNameSize;
    isSuccess = ReadFile(hPipe, fileName, MAX_PATH * sizeof(TCHAR), &fileNameSize, NULL);

    FILE* filePtr;
    _tfopen_s(&filePtr, fileName, _T("r, ccs=UTF-8"));
    if (filePtr == nullptr)
    {
        return -1;
    }

    DWORD bytesWritten = 0;
    DWORD bytesRead = 0;

    while (!feof(filePtr))
    {
        bytesRead = fread(dataBuf, 1, BUF_SIZE, filePtr);

        WriteFile(hPipe, dataBuf, bytesRead, &bytesWritten, NULL);

        if (bytesRead != bytesWritten)
        {
            break;
        }
    }

    FlushFileBuffers(hPipe);
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);

    return 1;
}
