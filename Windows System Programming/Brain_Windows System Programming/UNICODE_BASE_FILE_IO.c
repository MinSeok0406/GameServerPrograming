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

    TCHAR fileName[] = _T("data.txt");
    TCHAR fileData[] = _T("Just test string~");

    HANDLE hFile = CreateFile(fileName, GENERIC_WRITE, FILE_SHARE_WRITE,
        0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        _tprintf(_T("File creation fault! \n"));
        return -1;
    }

    DWORD numOfByteWritten = 0;
    WriteFile(hFile, fileData, sizeof(fileData), &numOfByteWritten, NULL);

    _tprintf(_T("Written data size: %u \n"), numOfByteWritten);

    CloseHandle(hFile);

    return 0;
}*/