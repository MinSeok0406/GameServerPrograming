#include <iostream>
#include <Windows.h>
#include <vector>
#include <time.h>
#include "Enemy.h"
#include "Player.h"
#include "Buffer.h"
#include "Console.h"
using namespace std;

extern vector<Enemy> enemies;
extern vector<pair<int, int>> Enemybullet;
extern bool isMove;

Player* p;
vector<pair<int, int>> Playerbullet;

int tempTick = timeGetTime();
int enemyCnt = 0;

void EnemyInit()
{
    // 텍스트 파일에서 값들을 가져와서 몬스터 배치
    enemies.push_back({ true, '@', 20, 2});
    enemies.push_back({ true, '@', 25, 2});
    enemies.push_back({ true, '@', 30, 2});
    enemies.push_back({ true, '@', 35, 2});
    enemies.push_back({ true, '@', 20, 5});
    enemies.push_back({ true, '@', 25, 5});
    enemies.push_back({ true, '@', 30, 5});
    enemies.push_back({ true, '@', 35, 5});
    enemyCnt = enemies.size();
}

void EnemyMovement()
{
    int t = timeGetTime() - tempTick;
    if (t > 0 && t < 2000)
    {
        isMove = true;
    }
    else if (t >= 2000 && t < 4000)
    {
        isMove = false;
    }
    else
    {
        isMove = true;
        tempTick += 4000;
    }
    
    if (isMove)
    {
        // 텍스트 파일에서 패턴 가져오기
        for (int i = 0; i < (int)enemies.size(); ++i)
        {
            if (enemies[i].live)
            {
                Sprite_Draw(enemies[i].x, enemies[i].y, enemies[i].sprite);
                enemies[i].x += 1;
            }
        }
    }
    else
    {
        // 텍스트 파일에서 패턴 가져오기
        for (int i = 0; i < (int)enemies.size(); ++i)
        {
            if (enemies[i].live)
            {
                Sprite_Draw(enemies[i].x, enemies[i].y, enemies[i].sprite);
                enemies[i].x -= 1;
            }
        }
    }
}

void EnemyAttack()
{
    // rand 함수로 확률로 공격하기
    int randValue = rand() % 100 + 1;
    
    // 적 총알 생성
    // 텍스트 파일에서 공격 확률 가져오기
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
        y = Enemybullet[i].second + 1;
        x = Enemybullet[i].first;
        if (p->x == x && p->y == y)
        {
            Enemybullet[i].first = -1;
            Enemybullet[i].second = -1;
            p->hp -= 20;
            if (p->hp <= 0)
            {
                p->live = false;
            }
        }
    }

    // 적 총알 이동
    for (int i = 0; i < (int)Enemybullet.size(); ++i)
    {
        if (Enemybullet[i].first != -1 && Enemybullet[i].second != -1)
        {
            Sprite_Draw(Enemybullet[i].first, Enemybullet[i].second, 'v');
            Enemybullet[i].second += 1;

            if (Enemybullet[i].second >= dfSCREEN_HEIGHT)
            {
                Enemybullet[i].first = -1;
                Enemybullet[i].second = -1;
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