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
//    HANDLE hProcess;
//    TCHAR cmdString[1024];
//
//    DuplicateHandle(GetCurrentProcess(), GetCurrentProcess(), GetCurrentProcess(),
//        &hProcess, 0, TRUE, DUPLICATE_SAME_ACCESS);
//
//    _stprintf(cmdString, _T("%s %u"), _T("Command_Prompt_Practice.exe"), (unsigned)hProcess);
//
//    STARTUPINFO si = {0, };
//    PROCESS_INFORMATION pi;
//    si.cb = sizeof(si);
//
//    BOOL isSuccess = CreateProcess(NULL, cmdString, NULL, NULL, TRUE,
//        CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
//
//    if (isSuccess == FALSE)
//    {
//        _tprintf(_T("CreateProcess failed \n"));
//        return -1;
//    }
//
//    CloseHandle(pi.hProcess);
//    CloseHandle(pi.hThread);
//
//    _tprintf(_T("[Parent Process]\n"));
//    _tprintf(_T("ooooooooooooooooooooooopps! \n"));
//
//    return 0;
//}