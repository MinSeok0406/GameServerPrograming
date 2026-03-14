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

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    TCHAR DirSpec[MAX_PATH];

    _tprintf(_T("Insert target directory: "));
    _tscanf(_T("%s"), DirSpec);
    _tcsncat(DirSpec, _T("\\*"), 3);

    hFind = FindFirstFile(DirSpec, &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        _tprintf(_T("Invalid file handle \n"));
        return -1;
    }
    else
    {
        _tprintf(_T("First file name is %s\n"), FindFileData.cFileName);
        while (FindNextFile(hFind, &FindFileData) != 0)
        {
            _tprintf(_T("Next file name is %s\n"), FindFileData.cFileName);
        }
        FindClose(hFind);
    }

    return 0;
}*/