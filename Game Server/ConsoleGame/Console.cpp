#include <windows.h>
#include <stdio.h>
#include "Console.h"

HANDLE hConsole;

void cs_Initial(void)
{
	CONSOLE_CURSOR_INFO stConsoleCursor;

	stConsoleCursor.bVisible = FALSE;
	stConsoleCursor.dwSize = 1;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE)
	{
		// 로그 남기기
		return;
	}

	auto result = SetConsoleCursorInfo(hConsole, &stConsoleCursor);
	if (result == 0)
	{
		// 로그 남기기
		return;
	}
}

void cs_MoveCursor(int iPosX, int iPosY)
{
	COORD stCoord;
	stCoord.X = iPosX;
	stCoord.Y = iPosY;

	auto result = SetConsoleCursorPosition(hConsole, stCoord);
	if (result == 0)
	{
		// 로그 남기기
		return;
	}
}

void cs_ClearScreen(void)
{
	DWORD dw;

	auto result = FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', 100 * 100, { 0, 0 }, &dw);
	if (result == 0)
	{
		// 로그 남기기
		return;
	}
}
