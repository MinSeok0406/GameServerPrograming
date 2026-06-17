#include <iostream>
#include <Windows.h>
#include <vector>
#include "EnemyObject.h"
#include "PlayerObject.h"
#include "ManagerObject.h"
#include "UpdateScene.h"
#include "Buffer.h"
#include "Console.h"
using namespace std;

extern ScreenBuffer* g_screenBuffer;
extern ManagerObject* g_managerObject;

//--------------------------------------

vector<Enemy> enemies;
vector<enemyBullet> Enemybullet;
bool isMove = true;

int tempTick = timeGetTime();
int enemyCnt = 0;

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

    enemyCnt = (int)enemies.size();
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
    return true;
}

EnemyObject::EnemyObject(bool live, bool isMove, char sprite, int x, int y, int moveX, int moveY)
    : _live(live), _isMove(isMove), _sprite(sprite), _x(x), _y(y), _moveX(moveX), _moveY(moveY),
    IBaseObject(OBJECT_TYPE::ENEMY)
{

}

EnemyObject::~EnemyObject()
{
}

bool EnemyObject::Update()
{
    return true;
}

void EnemyObject::Render()
{
    // 적 이동
    g_screenBuffer->Sprite_Draw(this->_x, this->_y, this->_sprite);

    if (this->_isMove)
    {
        this->_x += this->_moveX;
        this->_y += this->_moveY;
    }
    else
    {
        this->_x -= this->_moveX;
        this->_y -= this->_moveY;
    }
}

void EnemyObject::movement()
{
    int t = timeGetTime() - tempTick;
    if (t > 0 && t < 1000)
    {
        this->_isMove = true;
    }
    else if (t >= 1000 && t < 2000)
    {
        this->_isMove = false;
    }
    else
    {
        this->_isMove = true;
        tempTick += 2000;
    }
}

void EnemyObject::attack()
{
    // rand 함수로 확률로 공격하기
    int randValue = rand() % 100 + 1;

    // 적 총알 생성
    if (randValue > 80)
    {
        g_managerObject->CreateObject(OBJECT_TYPE::ENEMY_BULLET);
    }
}

bool EnemyObject::die()
{
    return false;
}
