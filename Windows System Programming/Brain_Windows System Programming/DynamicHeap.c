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

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    UINT pageSize = sysInfo.dwPageSize;

    HANDLE hHeap = HeapCreate(HEAP_NO_SERIALIZE, pageSize * 10, pageSize * 100);

    int* p = (int*)HeapAlloc(hHeap, 0, sizeof(int) * 10);

    for (int i = 0; i < 10; i++)
    {
        p[i] = i;
    }

    HeapFree(hHeap, 0, p);

    HeapDestroy(hHeap);

    HANDLE hDefaultHeap = GetProcessHeap();
    TCHAR* pDefault = (TCHAR*)HeapAlloc(hDefaultHeap, HEAP_NO_SERIALIZE,
        sizeof(TCHAR) * 30);

    _tcscpy(pDefault, _T("Default Heap!"));
    _tprintf(_T("%s \n"), pDefault);
    HeapFree(hDefaultHeap, HEAP_NO_SERIALIZE, pDefault);

    return 1;
}*/