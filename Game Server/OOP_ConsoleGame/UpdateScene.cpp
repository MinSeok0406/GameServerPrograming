#include "UpdateScene.h"
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include "Console.h"
#include "Buffer.h"
#include "ManagerObject.h"
using namespace std;

extern ScreenBuffer* g_screenBuffer;
extern ManagerObject* g_managerObject;
extern Console* g_console;

extern char buffer[256];
extern char enemyInfo[256][6];
extern int enemyTypeCnt;
extern char g_stageBuffer[LENGTH];
extern string str;

struct Enemy
{
    bool live;
    bool isMove;
    char sprite;
    int x;
    int y;
    int moveX;
    int moveY;
};

vector<Enemy> enemies;

char buffer[256];
int tick;
int g_stageBufferOffset = 0;

void EnemyInit()
{
    // Stage에 나오는 스프라이트 좌표 수집
    int x = 0;
    int y = 0;
    int cnt = 0;
    while (buffer[cnt] != '\0')
    {
        char c = buffer[cnt];

        if (c == '\r' || c == '\n')
        {
            if (c == '\r')
            {
                cnt += 2;
            }
            x = 0;
            y++;
            continue;
        }

        if (c == '@' || c == 'a' || c == 'Q' || c == 'W' || c == 'V')
        {
            enemies.push_back({ true, true, c, x, y, 0, 0 });
        }

        x++;
        cnt++;
    }

    // Enemy 이동 패턴 값 설정
    char sprite;
    int moveX;
    int moveY;

    for (auto i = 0; i < enemyTypeCnt; ++i)
    {
        sscanf_s(enemyInfo[i], " %c %d %d", &sprite, 1, &moveX, &moveY);

        for (auto& enemy : enemies)
        {
            if (enemy.sprite == sprite)
            {
                enemy.moveX = moveX;
                enemy.moveY = moveY;
            }
        }
    }

    for (auto& enemy : enemies)
    {
        g_managerObject->CreateObject(OBJECT_TYPE::ENEMY,
            enemy.live, enemy.isMove, enemy.sprite, enemy.x, enemy.y,
            enemy.moveX, enemy.moveY);
    }
}

void UpdateTitle()
{
    g_screenBuffer->Buffer_Clear();
    g_screenBuffer->Sprite_Draw(20, 0, 'G');
    g_screenBuffer->Sprite_Draw(22, 0, 'A');
    g_screenBuffer->Sprite_Draw(24, 0, 'M');
    g_screenBuffer->Sprite_Draw(26, 0, 'E');
    g_screenBuffer->Sprite_Draw(30, 0, 'R');
    g_screenBuffer->Sprite_Draw(32, 0, 'E');
    g_screenBuffer->Sprite_Draw(34, 0, 'A');
    g_screenBuffer->Sprite_Draw(36, 0, 'D');
    g_screenBuffer->Sprite_Draw(38, 0, 'Y');

    g_screenBuffer->Sprite_Draw(18, 2, 'P');
    g_screenBuffer->Sprite_Draw(20, 2, 'R');
    g_screenBuffer->Sprite_Draw(22, 2, 'E');
    g_screenBuffer->Sprite_Draw(24, 2, 'E');
    g_screenBuffer->Sprite_Draw(26, 2, 'S');
    g_screenBuffer->Sprite_Draw(30, 2, 'T');
    g_screenBuffer->Sprite_Draw(32, 2, 'A');
    g_screenBuffer->Sprite_Draw(34, 2, 'B');
    g_screenBuffer->Buffer_Flip();

    if (GetAsyncKeyState(VK_TAB))
    {
        g_stage = 1;
        //g_scene = SCENE::LOAD;
        g_console->cs_ClearScreen();
    }
}

void UpdateLoad()
{
    int maxStage = stoi(str);

    // 개발자가 만든 스테이지를 초과한다면...
    if (g_stage > maxStage)
    {
        //g_scene = SCENE::CLEAR;
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
        //g_scene = SCENE::CLEAR;
        return;
    }

    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    auto readBytes = fread(buffer, sizeof(char), fileSize, fp);
    buffer[readBytes] = '\0';

    fclose(fp);

    g_managerObject->CreateObject(OBJECT_TYPE::PLAYER);
    EnemyInit();

    //g_scene = SCENE::GAME;
    return;
}

void UpdateGame()
{
    tick = timeGetTime();

    // 10fps 게임으로 만든다.
    while (1)
    {
/*        // 게임 클리어
        if (EnemyDie())
        {
            g_scene = SCENE::LOAD;
            g_stage++;
            g_console->cs_ClearScreen();
            break;
        }

        // 플레이어 죽음
        if (PlayerDie())
        {
            g_scene = SCENE::OVER;
            g_console->cs_ClearScreen();
            break;
        }*/

        // 로직
        g_managerObject->Update();

        // 객체 삭제 관련 함수
        g_managerObject->RemoveObject();

        // 렌더링
        g_screenBuffer->Buffer_Clear();
        g_managerObject->Render();
        g_screenBuffer->Buffer_Flip();

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
    g_screenBuffer->Buffer_Clear();
    g_screenBuffer->Sprite_Draw(20, 0, 'G');
    g_screenBuffer->Sprite_Draw(22, 0, 'A');
    g_screenBuffer->Sprite_Draw(24, 0, 'M');
    g_screenBuffer->Sprite_Draw(26, 0, 'E');
    g_screenBuffer->Sprite_Draw(30, 0, 'C');
    g_screenBuffer->Sprite_Draw(32, 0, 'L');
    g_screenBuffer->Sprite_Draw(34, 0, 'E');
    g_screenBuffer->Sprite_Draw(36, 0, 'A');
    g_screenBuffer->Sprite_Draw(38, 0, 'R');

    g_screenBuffer->Sprite_Draw(20, 2, 'G');
    g_screenBuffer->Sprite_Draw(22, 2, 'O');
    g_screenBuffer->Sprite_Draw(26, 2, 'T');
    g_screenBuffer->Sprite_Draw(28, 2, 'I');
    g_screenBuffer->Sprite_Draw(30, 2, 'T');
    g_screenBuffer->Sprite_Draw(32, 2, 'L');
    g_screenBuffer->Sprite_Draw(34, 2, 'E');
    g_screenBuffer->Sprite_Draw(36, 2, '?');
    g_screenBuffer->Sprite_Draw(38, 2, '?');

    g_screenBuffer->Sprite_Draw(18, 4, 'P');
    g_screenBuffer->Sprite_Draw(20, 4, 'R');
    g_screenBuffer->Sprite_Draw(22, 4, 'E');
    g_screenBuffer->Sprite_Draw(24, 4, 'E');
    g_screenBuffer->Sprite_Draw(26, 4, 'S');
    g_screenBuffer->Sprite_Draw(30, 4, 'T');
    g_screenBuffer->Sprite_Draw(32, 4, 'A');
    g_screenBuffer->Sprite_Draw(34, 4, 'B');
    g_screenBuffer->Buffer_Flip();

    if (GetAsyncKeyState(VK_TAB))
    {
        //g_scene = SCENE::TITLE;
        g_stageBufferOffset = 0;
        g_console->cs_ClearScreen();
        return;
    }
}

void UpdateOver()
{
    g_screenBuffer->Buffer_Clear();
    g_screenBuffer->Sprite_Draw(20, 0, 'G');
    g_screenBuffer->Sprite_Draw(22, 0, 'A');
    g_screenBuffer->Sprite_Draw(24, 0, 'M');
    g_screenBuffer->Sprite_Draw(26, 0, 'E');
    g_screenBuffer->Sprite_Draw(30, 0, 'O');
    g_screenBuffer->Sprite_Draw(32, 0, 'V');
    g_screenBuffer->Sprite_Draw(34, 0, 'E');
    g_screenBuffer->Sprite_Draw(36, 0, 'R');

    g_screenBuffer->Sprite_Draw(18, 2, 'P');
    g_screenBuffer->Sprite_Draw(20, 2, 'R');
    g_screenBuffer->Sprite_Draw(22, 2, 'E');
    g_screenBuffer->Sprite_Draw(24, 2, 'E');
    g_screenBuffer->Sprite_Draw(26, 2, 'S');
    g_screenBuffer->Sprite_Draw(30, 2, 'T');
    g_screenBuffer->Sprite_Draw(32, 2, 'A');
    g_screenBuffer->Sprite_Draw(34, 2, 'B');
    g_screenBuffer->Buffer_Flip();

    if (GetAsyncKeyState(VK_TAB))
    {
        //g_scene = SCENE::GAME;
        g_console->cs_ClearScreen();
        return;
    }
}