/*#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <tchar.h>

static int total = 0;

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
    DWORD* nPtr = (DWORD*)lpParam;
    DWORD numOne = *nPtr;
    DWORD numTwo = *(nPtr + 1);

    for (DWORD i = numOne; i <= numTwo; i++)
    {
        total += i;
    }

    return 0;
}

int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, _T("korean"));

    DWORD dwThreadID[3];
    HANDLE hThread[3];

    DWORD paramThread[] = { 1, 10, 11, 80, 81, 120 };

    for (int i = 0; i < 3; i++)
    {
        hThread[i] = CreateThread(NULL, 0, ThreadProc, (LPVOID)(&paramThread[i * 2]),
            0, &dwThreadID[0]);

        if (hThread[i] == NULL)
        {
            _tprintf(_T("Thread creation fault! \n"));
            return -1;
        }
    }

    WaitForMultipleObjects(3, hThread, TRUE, INFINITE);

    _tprintf(_T("total (1 ~ 10) : %d \n"), total);

    for (int i = 0; i < 3; i++)
    {
        CloseHandle(hThread[i]);
    }

    return 0;
}*/