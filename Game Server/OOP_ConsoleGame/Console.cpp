#include "Console.h"

Console* Console::_pConsole = nullptr;

void Console::cs_Initial(void)
{
	CONSOLE_CURSOR_INFO stConsoleCursor;

	stConsoleCursor.bVisible = FALSE;
	stConsoleCursor.dwSize = 1;

	_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (_hConsole == INVALID_HANDLE_VALUE)
	{
		// 로그 남기기
		return;
	}

	auto result = SetConsoleCursorInfo(_hConsole, &stConsoleCursor);
	if (result == 0)
	{
		// 로그 남기기
		return;
	}
}

void Console::cs_MoveCursor(int iPosX, int iPosY)
{
	COORD stCoord;
	stCoord.X = iPosX;
	stCoord.Y = iPosY;

	auto result = SetConsoleCursorPosition(_hConsole, stCoord);
	if (result == 0)
	{
		// 로그 남기기
		return;
	}
}

void Console::cs_ClearScreen(void)
{
	DWORD dw;

	auto result = FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', 100 * 100, { 0, 0 }, &dw);
	if (result == 0)
	{
		// 로그 남기기
		return;
	}
}
