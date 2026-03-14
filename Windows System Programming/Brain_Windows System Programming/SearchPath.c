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

    TCHAR fileName[MAX_PATH];
    TCHAR bufFilePath[MAX_PATH];
    LPTSTR lpFilePart;
    DWORD result;

    _tprintf(_T("Insert name of the file to find: "));
    _tscanf(_T("%s"), fileName);

    result = SearchPath(NULL, fileName, NULL, MAX_PATH, bufFilePath, &lpFilePart);

    if (result == 0)
    {
        _tprintf(_T("File Not Found! \n"));
    }
    else
    {
        _tprintf(_T("File Path: %s \n"), bufFilePath);
        _tprintf(_T("File name only: %s \n"), lpFilePart);
    }

    return 0;
}*/