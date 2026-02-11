//#include <locale.h>
//#include <stdio.h>
//#include <string.h>
//#include <windows.h>
//#include <tchar.h>
//
//#define SLOT_NAME _T("\\\\.\\mailslot\\mailbox")
//
//int _tmain(int argc, TCHAR* argv[])
//{
//    _wsetlocale(LC_ALL, L"korean");
//
//    HANDLE hMailSolt;
//    TCHAR messageBox[50];
//    DWORD bytesRead;
//
//    hMailSolt = CreateMailslot(
//        SLOT_NAME,
//        0,
//        MAILSLOT_WAIT_FOREVER,
//        NULL);
//
//    if (hMailSolt == INVALID_HANDLE_VALUE)
//    {
//        _fputts(_T("Unable to create mailslot!\n"), stdout);
//        return 1;
//    }
//
//    _fputts(_T("******* Message *******\n"), stdout);
//    while (1)
//    {
//        if (!ReadFile(hMailSolt, messageBox, sizeof(TCHAR) * 50, &bytesRead, NULL))
//        {
//            _fputts(_T("Unable to read!"), stdout);
//            CloseHandle(hMailSolt);
//            return 1;
//        }
//
//        if (!_tcsncmp(messageBox, _T("exit"), 4))
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