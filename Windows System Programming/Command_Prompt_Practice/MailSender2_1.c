#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <tchar.h>

#define SLOT_NAME _T("\\\\.\\mailslot\\mailbox")

int _tmain(int argc, TCHAR* argv[])
{
    _wsetlocale(LC_ALL, L"korean");

    HANDLE hMailSolt;
    TCHAR message[50];
    DWORD bytesWritten;

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    hMailSolt = CreateFile(
        SLOT_NAME,
        GENERIC_WRITE,
        FILE_SHARE_READ,
        &sa,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hMailSolt == INVALID_HANDLE_VALUE)
    {
        _fputts(_T("Unable to create mailslot!"), stdout);
        return 1;
    }

    _tprintf(_T("Inheritable Handle : %p \n"), hMailSolt);
    FILE* file = _tfopen(_T("InheritableHandle.txt"), _T("wt"));
    _ftprintf(file, _T("%p"), hMailSolt);
    fclose(file);

    STARTUPINFO si = {0, };
    PROCESS_INFORMATION pi;
    si.cb = sizeof(si);

    TCHAR command[] = _T("ProcessTemp.exe");

    CreateProcess(NULL, command, NULL, NULL, TRUE,
        CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

    while (1)
    {
        _fputts(_T("MY CMD>"), stdout);
        _fgetts(message, sizeof(message) / sizeof(TCHAR), stdin);

        if (!WriteFile(hMailSolt, message, _tcslen(message) * sizeof(TCHAR), &bytesWritten, NULL))
        {
            _fputts(_T("Unable to write!"), stdout);
            CloseHandle(hMailSolt);
            return 1;
        }

        if (!_tcscmp(message, _T("exit")))
        {
            _fputts(_T("Good Bye!"), stdout);
            break;
        }
    }

    CloseHandle(hMailSolt);

    return 0;
}