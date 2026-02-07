//#define _CRT_SECURE_NO_WARNINGS
//#include <locale.h>
//#include <stdio.h>
//#include <string.h>
//#include <windows.h>
//#include <tchar.h>
//
//
//int _tmain(int argc, TCHAR* argv[])
//{
//    _wsetlocale(LC_ALL, L"korean");
//
//    HANDLE hReadPipe, hWritePipe;
//
//    TCHAR sendString[] = _T("anonymous pipe");
//    TCHAR recvString[100];
//
//    DWORD bytesWritten;
//    DWORD bytesRead;
//
//    CreatePipe(&hReadPipe, &hWritePipe, NULL, 0);
//
//    WriteFile(hWritePipe, sendString, lstrlen(sendString) * sizeof(TCHAR), &bytesWritten, NULL);
//    _tprintf(_T("string send: %s \n"), sendString);
//
//    ReadFile(hReadPipe, recvString, bytesWritten, &bytesRead, NULL);
//    recvString[bytesRead / sizeof(TCHAR)] = 0;
//    _tprintf(_T("string recv: %s \n"), recvString);
//
//    CloseHandle(hReadPipe);
//    CloseHandle(hWritePipe);
//
//    return 0;
//}