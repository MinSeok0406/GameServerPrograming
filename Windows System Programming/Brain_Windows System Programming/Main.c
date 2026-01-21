#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <tchar.h>


int _tmain(int argc, TCHAR* argv[])
{
    _wsetlocale(LC_ALL, L"korean");

    DWORD val1, val2;
    val1 = _ttoi(argv[1]);
    val2 = _ttoi(argv[2]);

    _tprintf(_T("%d + %d = %d \n"), val1, val2, val1 + val2);

    _gettchar();

    return 0;
}