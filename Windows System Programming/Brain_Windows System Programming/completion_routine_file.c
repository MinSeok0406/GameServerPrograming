/*#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <tchar.h>
#include <time.h>

TCHAR strData[] =
_T("Nobody was...\n")
_T("Royal Liverpool...\n")
_T("For the First...\n")
_T("Here's the solution...\n")
_T("fast fairways, penal...\n");


VOID WINAPI FileIOCompletionRoutine(DWORD, DWORD, LPOVERLAPPED);

int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, L"korean");

    TCHAR fileName[] = _T("data.txt");
    HANDLE hFile = CreateFile(fileName, GENERIC_WRITE, FILE_SHARE_WRITE,
        0, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, 0);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        _tprintf(_T("File creation fault! \n"));
        return -1;
    }

    OVERLAPPED overlappedInst;
    memset(&overlappedInst, 0, sizeof(overlappedInst));
    overlappedInst.hEvent = (HANDLE)1234;
    WriteFileEx(hFile, strData, sizeof(strData), &overlappedInst, FileIOCompletionRoutine);

    SleepEx(INFINITE, TRUE);
    CloseHandle(hFile);

    return 1;
}

VOID WINAPI FileIOCompletionRoutine(DWORD errorCode, DWORD numOfBytesTransfered, LPOVERLAPPED overlapped)
{
    _tprintf(_T("************** File Write result ******************\n"));
    _tprintf(_T("Error code: %u \n"), errorCode);
    _tprintf(_T("Transfered bytes len: %u \n"), numOfBytesTransfered);
    _tprintf(_T("the other info: %u \n"), (UINT_PTR)overlapped->hEvent);
}*/