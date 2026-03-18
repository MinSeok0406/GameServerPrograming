/*#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <tchar.h>
#include <time.h>

TCHAR strData1[] = _T("Nobody was...\n");
TCHAR strData2[] = _T("Royal Liverpool...\n");
TCHAR strData3[] = _T("For the First...\n");

VOID WINAPI FileIOCompletionRoutine(DWORD, DWORD, LPOVERLAPPED);

OVERLAPPED overlappedFilePtr;

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

    OVERLAPPED overlappedInstOne;
    memset(&overlappedInstOne, 0, sizeof(OVERLAPPED));
    overlappedInstOne.hEvent = (HANDLE)"First I/O";
    WriteFileEx(hFile, strData1, sizeof(strData1), &overlappedInstOne, FileIOCompletionRoutine);
    overlappedFilePtr.Offset += _tcslen(strData1) * sizeof(TCHAR);

    OVERLAPPED overlappedInstTwo;
    memset(&overlappedInstTwo, 0, sizeof(OVERLAPPED));
    overlappedInstTwo.Offset = overlappedFilePtr.Offset;
    overlappedInstTwo.hEvent = (HANDLE)"Second I/O";
    WriteFileEx(hFile, strData2, sizeof(strData2), &overlappedInstTwo, FileIOCompletionRoutine);
    overlappedFilePtr.Offset += _tcslen(strData2) * sizeof(TCHAR);

    OVERLAPPED overlappedInstThree;
    memset(&overlappedInstThree, 0, sizeof(OVERLAPPED));
    overlappedInstThree.hEvent = (HANDLE)"Third I/O";
    WriteFileEx(hFile, strData3, sizeof(strData3), &overlappedInstThree, FileIOCompletionRoutine);

    SleepEx(INFINITE, TRUE);
    CloseHandle(hFile);
    return 1;
}

VOID WINAPI FileIOCompletionRoutine(DWORD errorCode, DWORD numOfBytesTransfered, LPOVERLAPPED overlapped)
{
    _tprintf(_T("************** File Write result ******************\n"));
    _tprintf(_T("Error code: %u \n"), errorCode);
    _tprintf(_T("Transfered bytes len: %u \n"), numOfBytesTransfered);
    _tprintf(_T("the other info: %s \n"), (TCHAR*)overlapped->hEvent);
}*/