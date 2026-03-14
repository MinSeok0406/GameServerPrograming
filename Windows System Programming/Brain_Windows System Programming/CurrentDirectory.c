/*#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <tchar.h>
#include <time.h>

#define STRING_LEN 100
TCHAR fileData[] = _T("abcdefghijklmnopqrstuvwxyz");

int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, L"korean");

    TCHAR fileName[] = _T("data.txt");
    TCHAR readBuf[STRING_LEN];

    HANDLE hFileWrite;
    HANDLE hFileRead;
    DWORD numOfByteWritten;

    hFileWrite = CreateFile(fileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    WriteFile(hFileWrite, fileData, sizeof(fileData), &numOfByteWritten, NULL);
    CloseHandle(hFileWrite);

    //SetCurrentDirectory(_T("C:\\"));

    hFileRead = CreateFile(fileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    if (hFileRead == INVALID_HANDLE_VALUE)
    {
        _tprintf(_T("File open error! \n"));
        return -1;
    }

    ReadFile(hFileRead, readBuf, sizeof(readBuf), &numOfByteWritten, NULL);
    _tprintf(_T("%s \n"), readBuf);
    CloseHandle(hFileRead);

    return 0;
}*/