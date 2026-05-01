/*#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <locale.h>
#include <windows.h>

#define DIR_LEN MAX_PATH+1

int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, L"korean");

    STARTUPINFO si = {0, };
    PROCESS_INFORMATION pi;

    si.cb = sizeof(si);
    si.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE;
    si.dwX = 100;
    si.dwY = 200;
    si.dwXSize = 300;
    si.dwYSize = 200;
    si.lpTitle = _T("I am a boy!");

    TCHAR command[] = _T("Brain_Windows System Programming.exe 10 20");
    BOOL state;

    state = CreateProcess(
        NULL,
        command,
        NULL, NULL, TRUE,
        CREATE_NEW_CONSOLE,
        NULL, NULL, &si, &pi);

    if (state != 0)
    {
        _fputts(_T("Creation OK! \n"), stdout);
    }
    else
    {
        _fputts(_T("Creation Error! \n"), stdout);
    }

    return 0;
}*/