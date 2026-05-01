/*#define _WIN32_WINNT0x0400
#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <tchar.h>
#include <time.h>
#define MAX_PAGE 10

int* nextPageAddr;
DWORD pageCnt = 0;
DWORD pageSize;
int PageFaultExceptionFilter(DWORD);

int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, L"korean");

    LPVOID baseAddr;
    int* lpPtr;
    SYSTEM_INFO sSysInfo;

    GetSystemInfo(&sSysInfo);
    pageSize = sSysInfo.dwPageSize;

    baseAddr = VirtualAlloc(NULL, MAX_PAGE * pageSize, MEM_RESERVE, PAGE_NOACCESS);
    if (baseAddr == NULL)
    {
        _tprintf(_T("VirtualAlloc reserve failed"));
    }

    lpPtr = (int*)baseAddr;
    nextPageAddr = (int*)baseAddr;

    for (int i = 0; i < (MAX_PAGE * pageSize) / sizeof(int); i++)
    {
        __try
        {
            lpPtr[i] = i;
        }
        __except (PageFaultExceptionFilter(GetExceptionCode()))
        {
            ExitProcess(GetLastError());
        }
    }

    for (int i = 0; i < (MAX_PAGE * pageSize) / sizeof(int); i++)
    {
        _tprintf(_T("%d "), lpPtr[i]);
    }

    BOOL isSuccess = VirtualFree(baseAddr, 0, MEM_RELEASE);
    if (isSuccess)
    {
        _tprintf(_T("Release succeeded!"));
    }
    else
    {
        _tprintf(_T("Release failed!"));
    }

    return 1;
}

int PageFaultExceptionFilter(DWORD exptCode)
{
    if (exptCode != EXCEPTION_ACCESS_VIOLATION)
    {
        _tprintf(_T("Exception code = %d \n"), exptCode);
        return EXCEPTION_EXECUTE_HANDLER;
    }

    _tprintf(_T("Exception is a page fault \n"));

    if (pageCnt >= MAX_PAGE)
    {
        _tprintf(_T("Exception: out of pages \n"));
        return EXCEPTION_EXECUTE_HANDLER;
    }

    LPVOID lpvResult = VirtualAlloc((LPVOID)nextPageAddr, pageSize,
        MEM_COMMIT, PAGE_READWRITE);
    if (lpvResult == NULL)
    {
        _tprintf(_T("VirtualAlloc failed \n"));
        return EXCEPTION_EXECUTE_HANDLER;
    }
    else
    {
        _tprintf(_T("Allocating another page. \n"));
    }

    pageCnt++;
    nextPageAddr += (pageSize / sizeof(int));
    return EXCEPTION_CONTINUE_EXECUTION;
}*/