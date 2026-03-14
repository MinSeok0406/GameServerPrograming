/*#define _CRT_SECURE_NO_WARNINGS
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

    TCHAR sysPathBuf[MAX_PATH];
    TCHAR winPathBuf[MAX_PATH];

    GetSystemDirectory(sysPathBuf, MAX_PATH);
    GetWindowsDirectory(winPathBuf, MAX_PATH);

    _tprintf(_T("System dir: %s \n"), sysPathBuf);
    _tprintf(_T("Windows dir: %s \n"), winPathBuf);

    return 0;
}*/