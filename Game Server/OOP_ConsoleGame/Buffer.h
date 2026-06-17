#pragma once

#define dfSCREEN_WIDTH	81
#define dfSCREEN_HEIGHT	24

class ScreenBuffer
{
public:
	ScreenBuffer()
	{
		memset(_screenBuffer, 0, sizeof(_screenBuffer));
	}

	// 버퍼의 내용을 화면으로 찍어주는 함수
	void Buffer_Flip(void);

	// 화면 버퍼를 지우는 함수
	void Buffer_Clear(void);

	// 버퍼의 특정 위치에 원하는 문자를 출력
	void Sprite_Draw(int iX, int iY, char chSprite);

	static ScreenBuffer* getInstance()
	{
		if (_pScreenBuffer == nullptr)
		{
			_pScreenBuffer = new ScreenBuffer;
		}

		return _pScreenBuffer;
	}

	static void destoryInstance()
	{
		if (_pScreenBuffer != nullptr)
		{
			delete _pScreenBuffer;
			_pScreenBuffer = nullptr;
		}
	}

private:
	char _screenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];
	static ScreenBuffer* _pScreenBuffer;

	~ScreenBuffer() = default;
};
