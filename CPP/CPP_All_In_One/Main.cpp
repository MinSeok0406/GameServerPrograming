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

#pragma comment(lib, "Winmm.lib")

extern "C" const IMAGE_DOS_HEADER __ImageBase;

using ll = long long;



int _tmain(int argc, TCHAR* argv[])
{
    timeBeginPeriod(1);
    
    SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };

    sei.lpVerb = _T("runas");

    sei.lpFile = _T("cmd.exe");

    sei.nShow = SW_SHOWNORMAL;

    if (!ShellExecuteEx(&sei))
    {
        DWORD dwStatus = GetLastError();

        if (dwStatus == ERROR_CANCELLED)
        {

        }
        else
        {
            if (dwStatus == ERROR_FILE_NOT_FOUND)
            {

            }
        }
    }

    return 0;
}