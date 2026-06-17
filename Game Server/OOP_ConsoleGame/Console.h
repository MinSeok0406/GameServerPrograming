#pragma once
#include <windows.h>

class Console
{
public:
	// 콘솔 제어 준비 작업
	void cs_Initial(void);

	// 콘솔 화면의 커서를 X, Y 좌표로 이동
	void cs_MoveCursor(int iPosX, int iPosY);

	// 콘솔 화면을 초기화
	void cs_ClearScreen(void);

	static Console* getInstance()
	{
		if (_pConsole == nullptr)
		{
			_pConsole = new Console;
		}

		return _pConsole;
	}

	static void destoryInstance()
	{
		if (_pConsole != nullptr)
		{
			delete _pConsole;
			_pConsole = nullptr;
		}
	}

private:
	HANDLE _hConsole;
	static Console* _pConsole;

	Console() = default;
	~Console() = default;
};