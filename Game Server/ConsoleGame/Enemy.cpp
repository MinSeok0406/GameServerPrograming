#include <iostream>
#include <Windows.h>
#include <vector>
#include "Enemy.h"
#include "Player.h"
#include "UpdateScene.h"
#include "Buffer.h"
#include "Console.h"
using namespace std;

vector<Enemy> enemies;
vector<enemyBullet> Enemybullet;
bool isMove = true;

int tempTick = timeGetTime();
int enemyCnt = 0;

extern Player* p;
extern vector<playerBullet> Playerbullet;

extern char g_stageBuffer[LENGTH];
extern char g_enemyBuffer[LENGTH];
extern char buffer[256];
extern char enemyInfo[256][6];
extern int enemyTypeCnt;

void EnemyInit()
{
    isMove = true;

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
            enemies.push_back({ true, c, x, y, 0, 0 });
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
    
    enemyCnt = enemies.size();
}

void EnemyMovement()
{
    int t = timeGetTime() - tempTick;
    if (t > 0 && t < 1000)
    {
        isMove = true;
    }
    else if (t >= 1000 && t < 2000)
    {
        isMove = false;
    }
    else
    {
        isMove = true;
        tempTick += 2000;
    }
}

void EnemyAttack()
{
    // rand 함수로 확률로 공격하기
    int randValue = rand() % 100 + 1;
    
    // 적 총알 생성
    if (randValue > 80)
    {
        int enemyIndex = randValue % enemyCnt;

        // 살아있는 적만 총알 생성
        while (!enemies[enemyIndex].live)
        {
            enemyIndex = (enemyIndex + 1) % enemyCnt;
        }

        Enemybullet.push_back({ enemies[enemyIndex].x, enemies[enemyIndex].y + 1});
    }

    // 플레이어 피격 판정
    int y = 0;
    int x = 0;
    for (int i = 0; i < (int)Enemybullet.size(); ++i)
    {
        y = Enemybullet[i].y + 1;
        x = Enemybullet[i].x;
        if (p->x == x && p->y == y)
        {
            Enemybullet[i].x = -1;
            Enemybullet[i].y = -1;
            p->hp -= 20;
            if (p->hp <= 0)
            {
                p->live = false;
            }
        }
    }

    
}

void EnemyRendering()
{
    // 적 이동
    for (int i = 0; i < (int)enemies.size(); ++i)
    {
        if (enemies[i].live)
        {
            Sprite_Draw(enemies[i].x, enemies[i].y, enemies[i].sprite);

            if (isMove)
            {
                enemies[i].x += enemies[i].moveX;
                enemies[i].y += enemies[i].moveY;
            }
            else
            {
                enemies[i].x -= enemies[i].moveX;
                enemies[i].y -= enemies[i].moveY;
            }
        }
    }

    // 적 총알 이동
    for (int i = 0; i < (int)Enemybullet.size(); ++i)
    {
        if (Enemybullet[i].x != -1 && Enemybullet[i].y != -1)
        {
            Sprite_Draw(Enemybullet[i].x, Enemybullet[i].y, 'v');
            Enemybullet[i].y += 1;

            if (Enemybullet[i].y >= dfSCREEN_HEIGHT)
            {
                Enemybullet[i].x = -1;
                Enemybullet[i].y = -1;
            }
        }
    }
}

bool EnemyDie()
{
    for (int i = 0; i < (int)enemies.size(); ++i)
    {
        if (enemies[i].live)
        {
            return false;
        }
    }

    enemies.clear();
    Enemybullet.clear();
    isMove = true;
    Playerbullet.clear();
    delete p;
    return true;
}