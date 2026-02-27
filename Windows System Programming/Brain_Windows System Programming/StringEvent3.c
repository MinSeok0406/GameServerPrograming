/*#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <tchar.h>
#include <time.h>

unsigned int WINAPI OutputThreadFunction(LPVOID lpParam);
unsigned int WINAPI CountThreadFunction(LPVOID lpParam);

typedef struct _SynchString
{
    TCHAR string[100];
    HANDLE hEvent;
    HANDLE hMutex;
} SynchString;

SynchString gSynString;

int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, _T("korean"));

    HANDLE hThread[2];
    DWORD dwThreadID[2];

    gSynString.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    gSynString.hMutex = CreateMutex(NULL, FALSE, NULL);

    if (gSynString.hEvent == NULL || gSynString.hMutex == NULL)
    {
        _fputts(_T("kernel object creation error \n"), stdout);
        return -1;
    }

    hThread[0] = (HANDLE)_beginthreadex(NULL, 0,
        OutputThreadFunction, NULL, 0, (unsigned*)&dwThreadID[0]);

    hThread[1] = (HANDLE)_beginthreadex(NULL, 0,
        CountThreadFunction, NULL, 0, (unsigned*)&dwThreadID[1]);

    if (hThread[0] == 0 && hThread[1] == 0)
    {
        _fputts(_T("Thread creation error \n"), stdout);
        return -1;
    }

    _fputts(_T("Insert string: "), stdout);
    _fgetts(gSynString.string, 30, stdin);

    SetEvent(gSynString.hEvent);

    WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

    CloseHandle(gSynString.hEvent);
    CloseHandle(gSynString.hMutex);
    CloseHandle(hThread[0]);
    CloseHandle(hThread[1]);

    return 0;
}

unsigned int WINAPI OutputThreadFunction(LPVOID lpParam)
{
    WaitForSingleObject(gSynString.hEvent, INFINITE);
    WaitForSingleObject(gSynString.hMutex, INFINITE);

    _fputts(_T("Output string: "), stdout);
    _fputts(gSynString.string, stdout);

    ReleaseMutex(gSynString.hMutex);
    return 0;
}

unsigned int WINAPI CountThreadFunction(LPVOID lpParam)
{
    WaitForSingleObject(gSynString.hEvent, INFINITE);
    WaitForSingleObject(gSynString.hMutex, INFINITE);

    _tprintf(_T("output string length: %d \n"), (int)(_tcslen(gSynString.string) - 1));

    ReleaseMutex(gSynString.hMutex);
    return 0;
}*/