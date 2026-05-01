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

    SYSTEM_INFO si;
    DWORD allocGranularity;
    DWORD pageSize;

    GetSystemInfo(&si);
    pageSize = si.dwPageSize;
    allocGranularity = si.dwAllocationGranularity;

    _tprintf(_T("Page size : %u Kbyte \n"), pageSize / 1024);
    _tprintf(_T("Allocation granularity : %u Kbyte \n"), allocGranularity / 1024);

    return 1;
}*/