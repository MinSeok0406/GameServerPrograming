//#define _CRT_SECURE_NO_WARNINGS
//#include <locale.h>
//#include <stdio.h>
//#include <string.h>
//#include <windows.h>
//#include <tchar.h>
//
//#define SLOT_NAME _T("\\\\.\\mailslot\\mailbox")
//#define SLOT_NAME2 _T("\\\\.\\mailslot\\mailbox2")
//
//int _tmain(int argc, TCHAR* argv[])
//{
//    _wsetlocale(LC_ALL, L"korean");
//
//    HANDLE mMailSolt;
//    TCHAR messageBox[50];
//    DWORD bytesRead;
//
//    mMailSolt = CreateMailslot(
//        SLOT_NAME2,
//        0,
//        MAILSLOT_WAIT_FOREVER,
//        NULL);
//
//    if (mMailSolt == INVALID_HANDLE_VALUE)
//    {
//        _fputts(_T("Unable to create mailslot!\n"), stdout);
//        return 1;
//    }
//
//    HANDLE hMailSolt;
//    TCHAR message[50];
//    DWORD bytesWritten;
//
//    while (1)
//    {
//        hMailSolt = CreateFile(
//            SLOT_NAME,
//            GENERIC_ALL,
//            FILE_SHARE_READ,
//            NULL,
//            OPEN_EXISTING,
//            FILE_ATTRIBUTE_NORMAL,
//            NULL
//        );
//
//        if (hMailSolt != INVALID_HANDLE_VALUE)
//        {
//            break;
//        }
//    }
//
//    while (1)
//    {
//        //_fputts(_T("MY CMD>"), stdout);
//        _fgetts(message, sizeof(message) / sizeof(TCHAR), stdin);
//
//        if (!WriteFile(hMailSolt, message, _tcslen(message) * sizeof(TCHAR), &bytesWritten, NULL))
//        {
//            _fputts(_T("Unable to write!"), stdout);
//            CloseHandle(hMailSolt);
//            return 1;
//        }
//
//        if (!ReadFile(mMailSolt, messageBox, sizeof(TCHAR) * 50, &bytesRead, NULL))
//        {
//            _fputts(_T("Unable to read!"), stdout);
//            CloseHandle(mMailSolt);
//            return 1;
//        }
//
//        if (!_tcsncmp(messageBox, _T("exit"), 4))
//        {
//            _fputts(_T("Good Bye!"), stdout);
//            break;
//        }
//
//        if (!_tcscmp(message, _T("exit")))
//        {
//            _fputts(_T("Good Bye!"), stdout);
//            break;
//        }
//
//        messageBox[bytesRead / sizeof(TCHAR)] = 0;
//        _fputts(messageBox, stdout);
//    }
//
//    CloseHandle(hMailSolt);
//
//    return 0;
//}