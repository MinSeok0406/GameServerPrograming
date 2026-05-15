#include <iostream>
#include <Windows.h>
#include "Console.h"
#include "Buffer.h"
#include "UpdateScene.h"

char szScreenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

void UpdateTitle()
{
    Buffer_Clear();
    // 텍스트 파일 만들기
    Sprite_Draw(20, 0, 'G');
    Sprite_Draw(22, 0, 'A');
    Sprite_Draw(24, 0, 'M');
    Sprite_Draw(26, 0, 'E');
    Sprite_Draw(30, 0, 'R');
    Sprite_Draw(32, 0, 'E');
    Sprite_Draw(34, 0, 'A');
    Sprite_Draw(36, 0, 'D');
    Sprite_Draw(38, 0, 'Y');

    // 텍스트 파일 만들기
    Sprite_Draw(18, 2, 'P');
    Sprite_Draw(20, 2, 'R');
    Sprite_Draw(22, 2, 'E');
    Sprite_Draw(24, 2, 'E');
    Sprite_Draw(26, 2, 'S');
    Sprite_Draw(30, 2, 'S');
    Sprite_Draw(32, 2, 'P');
    Sprite_Draw(34, 2, 'A');
    Sprite_Draw(36, 2, 'C');
    Sprite_Draw(38, 2, 'E');

    Buffer_Flip();

    if (GetAsyncKeyState(VK_SPACE))
    {
        g_state = 1;
        g_scene = SCENE::LOAD;
        cs_ClearScreen();
    }
}

void UpdateLoad()
{
    // 스테이지에 맞는 정보 읽기
    // fopen_s, fread, fwrite 함수로 파일 정보 가져오기
    

    g_scene = SCENE::GAME;
}

void UpdateGame()
{

}

void UpdateClear()
{

}

void UpdateOver()
{

}