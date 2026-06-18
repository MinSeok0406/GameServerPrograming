#include <iostream>
#include <Windows.h>
#include <vector>
#include "EnemyObject.h"
#include "ManagerObject.h"
#include "Buffer.h"
using namespace std;

extern ScreenBuffer* g_screenBuffer;
extern ManagerObject* g_managerObject;

//--------------------------------------

int tempTick = timeGetTime();

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
    movement();
    attack();

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

bool EnemyObject::RemoveObject()
{
    if (die())
    {
        g_managerObject->DestoryObject(this);
        return true;
    }

    return false;
}

// _live 멤버변수 변경될 수 있는 함수
bool EnemyObject::OnCollision(IBaseObject* obj)
{
    return false;
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
        g_managerObject->CreateObject(OBJECT_TYPE::ENEMY_BULLET,
            this->_x, this->_y + 1);
    }
}

bool EnemyObject::die()
{
    if (this->_live == false)
    {
        return true;
    }
    else
    {
        return false;
    }
}
