//#define _CRT_SECURE_NO_WARNINGS
//#include <locale.h>
//#include <stdio.h>
//#include <string.h>
//#include <windows.h>
//#include <tchar.h>
//
//int _tmain(int argc, TCHAR* argv[])
//{
//    _wsetlocale(LC_ALL, L"korean");
//
//    SetEnvironmentVariable(_T("Good"), _T("morning"));
//    SetEnvironmentVariable(_T("Hey"), _T("Ho!"));
//    SetEnvironmentVariable(_T("Big"), _T("Boy"));
//
//    STARTUPINFO si = { 0, };
//    PROCESS_INFORMATION pi = {0, };
//    si.cb = sizeof(si);
//    TCHAR command[] = _T("ProcessTemp.exe");
//
//    CreateProcess(NULL, command, NULL, NULL,
//        FALSE, CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT,
//        NULL, NULL, &si, &pi);
//
//    CloseHandle(pi.hProcess);
//    CloseHandle(pi.hThread);
//
//    return 0;
//}