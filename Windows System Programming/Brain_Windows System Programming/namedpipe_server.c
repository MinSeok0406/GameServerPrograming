//#define _CRT_SECURE_NO_WARNINGS
//#include <locale.h>
//#include <stdio.h>
//#include <string.h>
//#include <windows.h>
//#include <tchar.h>
//
//#define BUF_SIZE 1024
//int CommToClient(HANDLE);
//
//int _tmain(int argc, TCHAR* argv[])
//{
//    _wsetlocale(LC_ALL, L"korean");
//
//    LPTSTR pipeName = _T("\\\\.\\pipe\\simple_pipe");
//    HANDLE hPipe;
//    while (1)
//    {
//        hPipe = CreateNamedPipe(
//            pipeName,
//            PIPE_ACCESS_DUPLEX,
//            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
//            PIPE_UNLIMITED_INSTANCES,
//            BUF_SIZE,
//            BUF_SIZE,
//            20000,
//            NULL);
//
//        if (hPipe == INVALID_HANDLE_VALUE)
//        {
//            _tprintf(_T("CreatePipe failed"));
//            return -1;
//        }
//
//        BOOL isSuccess = 0;
//        isSuccess = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
//
//        if (isSuccess)
//        {
//            CommToClient(hPipe);
//        }
//        else
//        {
//            CloseHandle(hPipe);
//        }
//    }
//
//    return 1;
//}
//
//int CommToClient(HANDLE hPipe)
//{
//    TCHAR fileName[MAX_PATH];
//    TCHAR dataBuf[BUF_SIZE];
//    DWORD fileNameBytes = 0;
//
//    BOOL isSuccess;
//    isSuccess = ReadFile(
//        hPipe,
//        fileName,
//        MAX_PATH * sizeof(TCHAR),
//        &fileNameBytes,
//        NULL);
//
//    if (!isSuccess || fileNameBytes == 0)
//    {
//        _tprintf(_T("Pipe read message error! \n"));
//        return -1;
//    }
//
//    DWORD fileNameChars = fileNameBytes / sizeof(TCHAR);
//    if (fileNameChars >= MAX_PATH)
//    {
//        fileNameChars = MAX_PATH - 1;
//    }
//    fileName[fileNameChars] = 0;
//
//    FILE* filePtr = _tfopen(fileName, _T("rt, ccs=UTF-8"));
//    if (filePtr == NULL)
//    {
//        _tprintf(_T("File open fault! \n"));
//        return -1;
//    }
//
//    DWORD bytesWritten = 0;
//
//    while (_fgetts(dataBuf, BUF_SIZE, filePtr) != NULL)
//    {
//        DWORD toWriteBytes = (DWORD)(_tcslen(dataBuf) * sizeof(TCHAR));
//
//        WriteFile(
//            hPipe,
//            dataBuf,
//            toWriteBytes,
//            &bytesWritten,
//            NULL);
//
//        if (toWriteBytes != bytesWritten)
//        {
//            _tprintf(_T("Pipe write message error! \n"));
//            break;
//        }
//    }
//
//    fclose(filePtr);
//    FlushFileBuffers(hPipe);
//    DisconnectNamedPipe(hPipe);
//    CloseHandle(hPipe);
//    return 1;
//}