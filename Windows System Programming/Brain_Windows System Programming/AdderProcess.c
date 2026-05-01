/*#define _WIN32_WINNT0x0400
#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <tchar.h>
#include <time.h>


int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, L"korean");

    DWORD val1, val2;
    val1 = _ttoi(argv[1]);
    val2 = _ttoi(argv[2]);

    _tprintf(_T("%d + %d = %d \n"), val1, val2, val1 + val2);

    _gettchar();

    return 1;
}*/