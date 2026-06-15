#include <iostream>
#include <string>
#include <Windows.h>
#include "Console.h"
#include "Buffer.h"
#include "UpdateScene.h"
#include "Player.h"
#include "Enemy.h"
using namespace std;

extern char szScreenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];
extern char g_stageBuffer[LENGTH];
extern string str;

char buffer[256];
int tick;
int t = timeGetTime();
int g_fpsCnt;
int g_stageBufferOffset = 0;

void FPS()
{
    g_fpsCnt++;

    if (timeGetTime() - t > 1000)
    {
        printf("FPS : %d\n", g_fpsCnt);
        printf("Move : ← ↑ → ↓ \t Attack : SPACE");
        g_fpsCnt = 0;
        t += 1000;
    }
}

void UpdateTitle()
{
    Buffer_Clear();
    Sprite_Draw(20, 0, 'G');
    Sprite_Draw(22, 0, 'A');
    Sprite_Draw(24, 0, 'M');
    Sprite_Draw(26, 0, 'E');
    Sprite_Draw(30, 0, 'R');
    Sprite_Draw(32, 0, 'E');
    Sprite_Draw(34, 0, 'A');
    Sprite_Draw(36, 0, 'D');
    Sprite_Draw(38, 0, 'Y');

    Sprite_Draw(18, 2, 'P');
    Sprite_Draw(20, 2, 'R');
    Sprite_Draw(22, 2, 'E');
    Sprite_Draw(24, 2, 'E');
    Sprite_Draw(26, 2, 'S');
    Sprite_Draw(30, 2, 'T');
    Sprite_Draw(32, 2, 'A');
    Sprite_Draw(34, 2, 'B');
    Buffer_Flip();

    if (GetAsyncKeyState(VK_TAB))
    {
        g_stage = 1;
        g_scene = SCENE::LOAD;
        cs_ClearScreen();
    }
}

void UpdateLoad()
{
    int maxStage = stoi(str);

    // 개발자가 만든 스테이지를 초과한다면...
    if (g_stage > maxStage)
    {
        g_scene = SCENE::CLEAR;
        return;
    }

    // 스테이지에 맞는 정보 읽기
    string arr;
    while (g_stageBuffer[g_stageBufferOffset] != '\0')
    {
        if (g_stageBuffer[g_stageBufferOffset] == '\0')
        {
            break;
        }

        char c = g_stageBuffer[g_stageBufferOffset];
        g_stageBufferOffset++;

        if (c == '\r')
        {
            if (g_stageBuffer[g_stageBufferOffset] == '\n')
            {
                g_stageBufferOffset++;
            }
            break;
        }
        arr += c;
    }

    FILE* fp;
    fopen_s(&fp, arr.c_str(), "rb");

    // 작성된 스테이지 텍스트 파일이 없다면...
    if (fp == nullptr)
    {
        g_scene = SCENE::CLEAR;
        return;
    }

    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    auto readBytes = fread(buffer, sizeof(char), fileSize, fp);
    buffer[readBytes] = '\0';

    fclose(fp);

    g_scene = SCENE::GAME;
    return;
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

        // 로직
        EnemyMovement();
        EnemyAttack();

        PlayerMoveMent();
        PlayerAttack();

        // 렌더링
        Buffer_Clear();

        EnemyRendering();
        PlayerRendering();

        Buffer_Flip();

        int useTime = (int)(timeGetTime() - tick);
        if (useTime > 0 && useTime < 100)
        {
            Sleep(100 - useTime);
        }
        tick += 100;
    }

    return;
}

void UpdateClear()
{
    Buffer_Clear();
    Sprite_Draw(20, 0, 'G');
    Sprite_Draw(22, 0, 'A');
    Sprite_Draw(24, 0, 'M');
    Sprite_Draw(26, 0, 'E');
    Sprite_Draw(30, 0, 'C');
    Sprite_Draw(32, 0, 'L');
    Sprite_Draw(34, 0, 'E');
    Sprite_Draw(36, 0, 'A');
    Sprite_Draw(38, 0, 'R');

    Sprite_Draw(20, 2, 'G');
    Sprite_Draw(22, 2, 'O');
    Sprite_Draw(26, 2, 'T');
    Sprite_Draw(28, 2, 'I');
    Sprite_Draw(30, 2, 'T');
    Sprite_Draw(32, 2, 'L');
    Sprite_Draw(34, 2, 'E');
    Sprite_Draw(36, 2, '?');
    Sprite_Draw(38, 2, '?');

    Sprite_Draw(18, 4, 'P');
    Sprite_Draw(20, 4, 'R');
    Sprite_Draw(22, 4, 'E');
    Sprite_Draw(24, 4, 'E');
    Sprite_Draw(26, 4, 'S');
    Sprite_Draw(30, 4, 'T');
    Sprite_Draw(32, 4, 'A');
    Sprite_Draw(34, 4, 'B');
    Buffer_Flip();

    if (GetAsyncKeyState(VK_TAB))
    {
        g_scene = SCENE::TITLE;
        g_stageBufferOffset = 0;
        cs_ClearScreen();
        return;
    }
}

void UpdateOver()
{
    Buffer_Clear();
    Sprite_Draw(20, 0, 'G');
    Sprite_Draw(22, 0, 'A');
    Sprite_Draw(24, 0, 'M');
    Sprite_Draw(26, 0, 'E');
    Sprite_Draw(30, 0, 'O');
    Sprite_Draw(32, 0, 'V');
    Sprite_Draw(34, 0, 'E');
    Sprite_Draw(36, 0, 'R');

    Sprite_Draw(18, 2, 'P');
    Sprite_Draw(20, 2, 'R');
    Sprite_Draw(22, 2, 'E');
    Sprite_Draw(24, 2, 'E');
    Sprite_Draw(26, 2, 'S');
    Sprite_Draw(30, 2, 'T');
    Sprite_Draw(32, 2, 'A');
    Sprite_Draw(34, 2, 'B');
    Buffer_Flip();

    if (GetAsyncKeyState(VK_TAB))
    {
        g_scene = SCENE::GAME;
        cs_ClearScreen();
        return;
    }
}