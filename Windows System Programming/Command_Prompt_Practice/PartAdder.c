#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <tchar.h>

int _tmain(int argc, TCHAR* argv[])
{
    _wsetlocale(LC_ALL, L"korean");

    if (argc != 3)
        return -1;

    DWORD start = _ttoi(argv[1]);
    DWORD end = _ttoi(argv[2]);

    DWORD total = 0;
    for (DWORD i = start; i <= end; i++)
    {
        total += i;
    }

    return total;
}