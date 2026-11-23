#include "CrashDump.h"
#include <stdio.h>
#include <crtdbg.h>
#include <dbghelp.h>

#pragma comment(lib, "DbgHelp.Lib")

long CrashDump::_dumpCount = 0;
extern long g_stopLog;

CrashDump::CrashDump()
{
	_invalid_parameter_handler oldHandler;
	_invalid_parameter_handler newHandler = myInvalidParameterHandler;

	oldHandler = _set_invalid_parameter_handler(newHandler);
	_CrtSetReportMode(_CRT_WARN, 0);
	_CrtSetReportMode(_CRT_ERROR, 0);
	_CrtSetReportMode(_CRT_ASSERT, 0);

	_CrtSetReportHook(custom_Report_Hook);
	_set_purecall_handler(myPurecallHandler);

	setHandlerDump();
}

void CrashDump::Crash()
{
	InterlockedExchange(&g_stopLog, 1);
	int* p = nullptr;
	*p = 0;
}

long __stdcall CrashDump::myExceptionFilter(PEXCEPTION_POINTERS pExceptionPointer)
{
	SYSTEMTIME stNowTime;

	InterlockedExchange(&g_stopLog, 1);
	long dumpCount = InterlockedIncrement(&_dumpCount);

	wchar_t filename[MAX_PATH];
	
	GetLocalTime(&stNowTime);
	wsprintf(filename, L"Dump_%d%02d%02d_%02d.%02d.%02d_%d.dmp",
		stNowTime.wYear, stNowTime.wMonth, stNowTime.wDay, stNowTime.wHour, stNowTime.wMinute, stNowTime.wSecond, dumpCount);

	wprintf(L"\n\n\n!!! Crash Error !!! %d.%d.%d / %d:%d:%d \n",
		stNowTime.wYear, stNowTime.wMonth, stNowTime.wDay, stNowTime.wHour, stNowTime.wMinute, stNowTime.wSecond);
	wprintf(L"Now Save dump file...\n");

	HANDLE hDumpFile = ::CreateFile(filename,
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hDumpFile != INVALID_HANDLE_VALUE)
	{
		_MINIDUMP_EXCEPTION_INFORMATION MinidumpExceptionInformation;
		
		MinidumpExceptionInformation.ThreadId = ::GetCurrentThreadId();
		MinidumpExceptionInformation.ExceptionPointers = pExceptionPointer;
		MinidumpExceptionInformation.ClientPointers = TRUE;

		MiniDumpWriteDump(GetCurrentProcess(),
			GetCurrentProcessId(),
			hDumpFile,
			MiniDumpWithFullMemory,
			&MinidumpExceptionInformation,
			NULL,
			NULL);

		CloseHandle(hDumpFile);

		wprintf(L"CrashDump Save Finish !");
	}

	return EXCEPTION_EXECUTE_HANDLER;
}

void CrashDump::setHandlerDump()
{
	SetUnhandledExceptionFilter(myExceptionFilter);
}

void CrashDump::myInvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, UINT32 line, uintptr_t pReserved)
{
	Crash();
}

int CrashDump::custom_Report_Hook(int ireposttype, char* message, int* returnvalue)
{
	Crash();
	return true;
}

void CrashDump::myPurecallHandler()
{
	Crash();
}