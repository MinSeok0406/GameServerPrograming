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

int _tmain(int argc, TCHAR* argv[])
{
    timeBeginPeriod(1);
    system("chcp 65001");

    HANDLE hPipe;
    TCHAR readDataBuf[BUF_SIZE + 1];
    LPCTSTR pipeName = _T("\\\\.\\pipe\\simple_pipe");

    while (1)
    {
        hPipe = CreateFile(pipeName, GENERIC_READ | GENERIC_WRITE,
            0, NULL, OPEN_EXISTING, 0, NULL);

        if (hPipe != INVALID_HANDLE_VALUE)
        {
            break;
        }

        if (GetLastError() != ERROR_PIPE_BUSY)
        {
            return 0;
        }

        if (!WaitNamedPipe(pipeName, 20000))
        {
            return 0;
        }
    }

    DWORD pipeMode = PIPE_READMODE_MESSAGE | PIPE_WAIT;
    BOOL isSuccess = SetNamedPipeHandleState(hPipe, &pipeMode, NULL, NULL);

    if (!isSuccess)
    {
        return 0;
    }

    LPCTSTR fileName = _T("news.txt");
    DWORD bytesWritten = 0;

    isSuccess = WriteFile(hPipe, fileName, (_tcslen(fileName) + 1), &bytesWritten, NULL);
    if (!isSuccess)
    {
        return 0;
    }

    DWORD bytesRead = 0;
    while (1)
    {
        isSuccess = ReadFile(hPipe, readDataBuf, BUF_SIZE * sizeof(TCHAR), &bytesRead, NULL);

        if (!isSuccess && (GetLastError() != ERROR_MORE_DATA))
        {
            break;
        }

        readDataBuf[bytesRead] = 0;
        tcout << readDataBuf << "\n";
    }

    CloseHandle(hPipe);
    
    return 0;
}