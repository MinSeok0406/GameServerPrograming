/*#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <tchar.h>
#include <time.h>

void ShowAttributes(DWORD attrib);

int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, L"korean");

    TCHAR fileName[] = _T("data.txt");

    _tprintf(_T("Original file attributes \n"));
    DWORD attrib = GetFileAttributes(_T("data.txt"));
    ShowAttributes(attrib);

    attrib |= (FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN);
    SetFileAttributes(_T("data.txt"), attrib);

    _tprintf(_T("Changed file attributes \n"));
    attrib = GetFileAttributes(_T("data.txt"));
    ShowAttributes(attrib);

    return 0;
}

void ShowAttributes(DWORD attrib)
{
    if (attrib & FILE_ATTRIBUTE_NORMAL)
    {
        _tprintf(_T("Normal \n"));
    }
    else
    {
        if (attrib & FILE_ATTRIBUTE_READONLY)
        {
            _tprintf(_T("Read Only \n"));
        }

        if (attrib & FILE_ATTRIBUTE_HIDDEN)
        {
            _tprintf(_T("Hidden \n"));
        }
    }
    _tprintf(_T("\n"));
}*/