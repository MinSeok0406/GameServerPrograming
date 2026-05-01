/*#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <tchar.h>

int _tmain(int argc, TCHAR* argv[])
{
    _wsetlocale(LC_ALL, L"korean");

    SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);

    while (1)
    {
        for (DWORD i = 0; i < 10000; i++)
        {
            for (DWORD j = 0; j < 10000; j++)
            {
                
            }
        }

        Sleep(10);

        _fputts(_T("BELOW_NORMAL_PRIORITY_CLASS Process"), stdout);
    }

    return 0;
}*/