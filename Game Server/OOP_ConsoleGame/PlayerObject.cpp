#include <Windows.h>
#include "PlayerObject.h"
#include "ManagerObject.h"
#include "Buffer.h"
using namespace std;

extern ScreenBuffer* g_screenBuffer;
extern ManagerObject* g_managerObject;

PlayerObject::PlayerObject(int live, int hp, int x, int y)
	: _live(live), _hp(hp), _x(x), _y(y), IBaseObject(OBJECT_TYPE::PLAYER)
{
}

PlayerObject::~PlayerObject()
{
}

bool PlayerObject::Update()
{
	movement();
	attack();

	return true;
}

void PlayerObject::Render()
{
	// 플레이어 이동
	g_screenBuffer->Sprite_Draw(this->_x, this->_y, 'A');
}

bool PlayerObject::RemoveObject()
{
	if (die())
	{
		g_managerObject->DestoryObject(this);
		return true;
	}

	return false;
}

// _live 멤버변수 변경될 수 있는 함수
bool PlayerObject::OnCollision(IBaseObject* obj)
{
	return false;
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
	if (this->_live == false)
	{
		return true;
	}
	else
	{
		return false;
	}
}