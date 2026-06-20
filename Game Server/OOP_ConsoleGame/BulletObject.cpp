#include "BulletObject.h"
#include "ManagerObject.h"
#include "Buffer.h"

extern ScreenBuffer* g_screenBuffer;
extern ManagerObject* g_managerObject;

BulletObject::BulletObject(int x, int y, OBJECT_TYPE objType)
	: _live(true), IBaseObject(objType, x, y)
{
}

BulletObject::~BulletObject()
{
}

bool BulletObject::Update()
{
	if (this->GetObjectType() == OBJECT_TYPE::PLAYER_BULLET)
	{
		this->_y -= 1;

		if (this->_y < 0)
		{
			this->_live = false;
		}
	}
	else if (this->GetObjectType() == OBJECT_TYPE::ENEMY_BULLET)
	{
		this->_y += 1;

		if (this->_y >= dfSCREEN_HEIGHT)
		{
			this->_live = false;
		}
	}

	return true;
}

void BulletObject::Render()
{
	if (this->GetObjectType() == OBJECT_TYPE::PLAYER_BULLET)
	{
		g_screenBuffer->Sprite_Draw(this->_x, this->_y, '^');
	}
	else if (this->GetObjectType() == OBJECT_TYPE::ENEMY_BULLET)
	{
		g_screenBuffer->Sprite_Draw(this->_x, this->_y, 'v');
	}
}

bool BulletObject::RemoveObject()
{
	if (this->_live)
	{
		return false;
	}

	g_managerObject->DestoryObject(this);

	return true;
}

// 플레이어 혹은 적이 변수로 들어옴
bool BulletObject::OnCollision(IBaseObject* obj)
{
	// 플레이어 혹은 적 총알과의 충돌은 무시
	if (obj->GetObjectType() == OBJECT_TYPE::ENEMY_BULLET
		|| obj->GetObjectType() == OBJECT_TYPE::PLAYER_BULLET)
	{
		return false;
	}

	this->_live = false;
	return true;
}
