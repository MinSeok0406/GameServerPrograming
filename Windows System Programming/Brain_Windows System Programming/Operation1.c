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
//    STARTUPINFO si = { 0, };
//    PROCESS_INFORMATION pi;
//    si.cb = sizeof(si);
//    TCHAR command[] = _T("Command_Prompt_Practice.exe");
//
//    CreateProcess(NULL, command, NULL, NULL, TRUE,
//        0, NULL, NULL, &si, &pi);
//
//    while (1)
//    {
//        for (DWORD i = 0; i < 10000; i++)
//        {
//            for (DWORD j = 0; j < 10000; j++)
//            {
//
//            }
//        }
//
//        _fputts(_T("Operation1.exe \n"), stdout);
//    }
//
//    return 0;
//}