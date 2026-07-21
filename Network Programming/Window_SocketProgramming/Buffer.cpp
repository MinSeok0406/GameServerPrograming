#include <iostream>
#include "Buffer.h"
#include "Console.h"

wchar_t szScreenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

void Buffer_Flip(void)
{
    int iCnt;
    for (iCnt = 0; iCnt < dfSCREEN_HEIGHT; ++iCnt)
    {
        cs_MoveCursor(0, iCnt);
        wprintf(szScreenBuffer[iCnt]);
    }
}

void Buffer_Clear(void)
{
    int iCnt;
    wmemset(szScreenBuffer[0], L' ', dfSCREEN_WIDTH * dfSCREEN_HEIGHT);

    for (iCnt = 0; iCnt < dfSCREEN_HEIGHT; ++iCnt)
    {
        szScreenBuffer[iCnt][dfSCREEN_WIDTH - 1] = L'\0';
    }
}

void Sprite_Draw(int iX, int iY, wchar_t chSprite)
{
    if (iX < 0 || iY < 0 || iX >= dfSCREEN_WIDTH - 1 || iY >= dfSCREEN_HEIGHT)
    {
        return;
    }

    szScreenBuffer[iY][iX] = chSprite;
}