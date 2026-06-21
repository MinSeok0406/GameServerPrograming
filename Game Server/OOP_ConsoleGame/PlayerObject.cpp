#include <Windows.h>
#include "PlayerObject.h"
#include "ManagerObject.h"
#include "SceneManager.h"
#include "Console.h"
#include "Buffer.h"
using namespace std;

extern Console* g_console;
extern SceneManager* g_sceneManager;
extern ScreenBuffer* g_screenBuffer;
extern ManagerObject* g_managerObject;

PlayerObject::PlayerObject(int live, int hp, int x, int y)
	: _hp(hp), IBaseObject(OBJECT_TYPE::PLAYER, x, y, live)
{
}

PlayerObject::~PlayerObject()
{
}

bool PlayerObject::Update()
{
	this->movement();
	this->attack();

	return true;
}

void PlayerObject::Render()
{
	// 플레이어 이동
	g_screenBuffer->Sprite_Draw(this->_x, this->_y, 'A');
}

bool PlayerObject::RemoveObject()
{
	if (this->die())
	{
		g_sceneManager->loadScene(SCENE::OVER);
		g_console->cs_ClearScreen();

		return true;
	}

	return false;
}

// _live 멤버변수 변경될 수 있는 함수
// 적 총알이 무조건 변수로 들어옴
bool PlayerObject::OnCollision(IBaseObject* obj)
{
	if (obj->GetObjectType() != OBJECT_TYPE::ENEMY_BULLET)
	{
		return false;
	}

	this->_hp -= 10;
	if (this->_hp <= 0)
	{
		this->_live = false;
	}

	return true;
}

void PlayerObject::movement()
{
	if (GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_UP))
	{
		this->_x -= 1;
		this->_y -= 1;
	}
	else if (GetAsyncKeyState(VK_UP) && GetAsyncKeyState(VK_RIGHT))
	{
		this->_x += 1;
		this->_y -= 1;
	}
	else if (GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_DOWN))
	{
		this->_x += 1;
		this->_y += 1;
	}
	else if (GetAsyncKeyState(VK_DOWN) && GetAsyncKeyState(VK_LEFT))
	{
		this->_x -= 1;
		this->_y += 1;
	}
	else if (GetAsyncKeyState(VK_LEFT))
	{
		this->_x -= 1;
	}
	else if (GetAsyncKeyState(VK_UP))
	{
		this->_y -= 1;
	}
	else if (GetAsyncKeyState(VK_RIGHT))
	{
		this->_x += 1;
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		this->_y += 1;
	}
}

void PlayerObject::attack()
{
	// 총알 생성
	if (GetAsyncKeyState(VK_SPACE))
	{
		g_managerObject->CreateObject(OBJECT_TYPE::PLAYER_BULLET,
			this->_x, this->_y - 1);
	}
}

bool PlayerObject::die()
{
	return !this->_live;
}