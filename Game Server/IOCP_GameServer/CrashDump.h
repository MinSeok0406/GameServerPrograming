#pragma once
#include <Windows.h>


class CrashDump
{
public:
	CrashDump();

	static void Crash();
	static long WINAPI myExceptionFilter(PEXCEPTION_POINTERS pExceptionPointer);
	static void setHandlerDump();
	static void myInvalidParameterHandler(const wchar_t* expression,
		const wchar_t* function, const wchar_t* file, UINT32 line, uintptr_t pReserved);
	static int custom_Report_Hook(int ireposttype, char* message, int* returnvalue);
	static void myPurecallHandler();

private:
	static long _dumpCount;
};