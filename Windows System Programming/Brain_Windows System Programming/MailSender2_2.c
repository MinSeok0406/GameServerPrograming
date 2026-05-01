#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <tchar.h>

int _tmain(int argc, TCHAR* argv[])
{
    _wsetlocale(LC_ALL, L"korean");

    HANDLE hMailSlot;
    TCHAR message[50];
    DWORD bytesWritten;

    // 핸들을 얻는 코드
    TCHAR handleStr[64] = { 0 };
    GetEnvironmentVariable(_T("Handle"), handleStr, _countof(handleStr));
    _stscanf_s(handleStr, _T("%p"), &hMailSlot);

    _tprintf(_T("Inheritable Handle : %p \n"), hMailSlot);

    while (1)
    {
        _fputts(_T("MY CMD>"), stdout);
        _fgetts(message, sizeof(message) / sizeof(TCHAR), stdin);

        if (!WriteFile(hMailSlot, message, _tcslen(message) * sizeof(TCHAR), &bytesWritten, NULL))
        {
            _fputts(_T("Unable to write!"), stdout);
            _gettchar();
            CloseHandle(hMailSlot);
            return 1;
        }

        if (!_tcscmp(message, _T("exit")))
        {
            _fputts(_T("Good Bye!"), stdout);
            break;
        }
    }

    CloseHandle(hMailSlot);

    return 0;
}