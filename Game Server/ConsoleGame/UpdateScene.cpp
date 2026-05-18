#include <iostream>
#include <Windows.h>
#include "Console.h"
#include "Buffer.h"
#include "UpdateScene.h"
#include "Player.h"
#include "Enemy.h"

char szScreenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];
char g_stageBuffer[LENGTH];

int tick;
int t = timeGetTime();
int g_fpsCnt;

void FPS()
{
    g_fpsCnt++;

    if (timeGetTime() - t > 1000)
    {
        printf("FPS : %d", g_fpsCnt);
        g_fpsCnt = 0;
        t += 1000;
    }
}

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
        g_stage = 1;
        g_scene = SCENE::LOAD;
        cs_ClearScreen();
    }
}

void UpdateLoad()
{
    // 개발자가 만든 스테이지를 초과한다면...
    if (g_stage > 1)
    {
        g_scene = SCENE::CLEAR;
        return;
    }

    // 스테이지에 맞는 정보 읽기
    // fopen_s, fread, fwrite 함수로 파일 정보 가져오기

    g_scene = SCENE::GAME;
}

void UpdateGame()
{
    PlayerInit();
    EnemyInit();
    tick = timeGetTime();

    // 10fps 게임으로 만든다.
    while (1)
    {
        FPS();

        // 게임 클리어
        if (EnemyDie())
        {
            g_scene = SCENE::LOAD;
            g_stage++;
            cs_ClearScreen();
            break;
        }

        // 플레이어 죽음
        if (PlayerDie())
        {
            g_scene = SCENE::OVER;
            cs_ClearScreen();
            break;
        }

        Buffer_Clear();

        EnemyMovement();
        EnemyAttack();

        PlayerMoveMent();
        PlayerAttack();

        Buffer_Flip();

        int useTime = (int)(timeGetTime() - tick);
        if (useTime > 0 && useTime < 100)
        {
            Sleep(100 - useTime);
        }
        tick += 100;
    }
}

void UpdateClear()
{
    Buffer_Clear();
    // 텍스트 파일 만들기
    Sprite_Draw(20, 0, 'G');
    Sprite_Draw(22, 0, 'A');
    Sprite_Draw(24, 0, 'M');
    Sprite_Draw(26, 0, 'E');
    Sprite_Draw(30, 0, 'C');
    Sprite_Draw(32, 0, 'L');
    Sprite_Draw(34, 0, 'E');
    Sprite_Draw(36, 0, 'A');
    Sprite_Draw(38, 0, 'R');

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
        g_scene = SCENE::TITLE;
        cs_ClearScreen();
    }
}

void UpdateOver()
{
    Buffer_Clear();
    // 텍스트 파일 만들기
    Sprite_Draw(20, 0, 'G');
    Sprite_Draw(22, 0, 'A');
    Sprite_Draw(24, 0, 'M');
    Sprite_Draw(26, 0, 'E');
    Sprite_Draw(30, 0, 'O');
    Sprite_Draw(32, 0, 'V');
    Sprite_Draw(34, 0, 'E');
    Sprite_Draw(36, 0, 'R');

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
        g_scene = SCENE::GAME;
        cs_ClearScreen();
    }
}