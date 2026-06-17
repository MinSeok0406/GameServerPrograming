#include <iostream>
#include "Buffer.h"
#include "Console.h"

ScreenBuffer* ScreenBuffer::_pScreenBuffer = nullptr;

extern Console* g_console;

void ScreenBuffer::Buffer_Flip(void)
{
    int iCnt;
    for (iCnt = 0; iCnt < dfSCREEN_HEIGHT; ++iCnt)
    {
        g_console->cs_MoveCursor(0, iCnt);
        printf(_screenBuffer[iCnt]);
    }
}

void ScreenBuffer::Buffer_Clear(void)
{
    int iCnt;
    memset(_screenBuffer, ' ', dfSCREEN_WIDTH * dfSCREEN_HEIGHT);

    for (iCnt = 0; iCnt < dfSCREEN_HEIGHT; ++iCnt)
    {
        _screenBuffer[iCnt][dfSCREEN_WIDTH - 1] = '\0';
    }
}

void ScreenBuffer::Sprite_Draw(int iX, int iY, char chSprite)
{
    if (iX < 0 || iY < 0 || iX >= dfSCREEN_WIDTH - 1 || iY >= dfSCREEN_HEIGHT)
    {
        return;
    }

    _screenBuffer[iY][iX] = chSprite;
}
