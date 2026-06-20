#include "EnemyObject.h"
#include <iostream>
#include <Windows.h>
#include "Console.h"
#include "SceneManager.h"
#include "ManagerObject.h"
#include "Buffer.h"
using namespace std;

extern Console* g_console;
extern SceneManager* g_sceneManager;
extern ScreenBuffer* g_screenBuffer;
extern ManagerObject* g_managerObject;
extern int g_stage;

int tempTick = timeGetTime();

EnemyObject::EnemyObject(bool live, bool isMove, char sprite, int x, int y, int moveX, int moveY)
    : _live(live), _isMove(isMove), _sprite(sprite), _moveX(moveX), _moveY(moveY),
    IBaseObject(OBJECT_TYPE::ENEMY, x, y)
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
}

bool EnemyObject::RemoveObject()
{
    if (die())
    {
        g_managerObject->DestoryObject(this);
        
        // 오브젝트 리스트에 객체가 없다면...
        if (g_managerObject->_objectList.empty())
        {
            g_sceneManager->loadScene(SCENE::OVER);
            g_console->cs_ClearScreen();
            return true;
        }

        // 적이 필드에 있는지 검사
        for (auto& objList : g_managerObject->_objectList)
        {
            if (objList->GetObjectType() == OBJECT_TYPE::ENEMY)
            {
                return false;
            }
        }

        g_sceneManager->loadScene(SCENE::LOAD);
        g_stage++;
        g_console->cs_ClearScreen();
        return true;
    }

    return false;
}

// _live 멤버변수 변경될 수 있는 함수
// 플레이어 총알이 무조건 변수로 들어옴
bool EnemyObject::OnCollision(IBaseObject* obj)
{
    // 플레이어 총알이 아니라면 수행 안함
    if (obj->GetObjectType() != OBJECT_TYPE::PLAYER_BULLET)
    {
        return false;
    }

    this->_live = false;

    return true;
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

void EnemyObject::attack()
{
    // rand 함수로 확률로 공격하기
    int randValue = rand() % 100 + 1;

    // 적 총알 생성
    if (randValue > 90)
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
