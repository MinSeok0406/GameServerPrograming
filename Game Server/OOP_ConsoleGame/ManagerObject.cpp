#include "ManagerObject.h"
#include "PlayerObject.h"
#include "EnemyObject.h"
#include "BulletObject.h"

ManagerObject* ManagerObject::_pManagerObject = nullptr;

void ManagerObject::CreateObject(OBJECT_TYPE objType)
{
	IBaseObject* ibo;

	if (objType == OBJECT_TYPE::PLAYER)
	{
		ibo = new PlayerObject(true, 100, 40, 20);
	}
	else if (objType == OBJECT_TYPE::ENEMY)
	{
		ibo = new EnemyObject(true, true, ' ', 0, 0, 0, 0);
	}
	else if (objType == OBJECT_TYPE::PLAYER_BULLET)
	{
		// 플레이어 쪽에서 위치 줘야됨
		ibo = new BulletObject(0, 0, OBJECT_TYPE::PLAYER_BULLET);
	}
	else if (objType == OBJECT_TYPE::ENEMY_BULLET)
	{
		// 적 쪽에서 위치 줘야됨
		ibo = new BulletObject(0, 0, OBJECT_TYPE::ENEMY_BULLET);
	}

	_objectList.push_back(ibo);
}

void ManagerObject::DestoryObject(IBaseObject* obj)
{
	std::erase(_objectList, obj);

	if (obj->GetObjectType() == OBJECT_TYPE::PLAYER)
	{
		delete obj;
	}
	else if (obj->GetObjectType() == OBJECT_TYPE::ENEMY)
	{
		delete obj;
	}
	else if (obj->GetObjectType() == OBJECT_TYPE::PLAYER_BULLET
		|| obj->GetObjectType() == OBJECT_TYPE::ENEMY_BULLET)
	{
		delete obj;
	}
}

bool ManagerObject::Update()
{
	for (auto& obj : _objectList)
	{
		if (!obj->Update())
		{
			// 예외처리
		}
	}
}

void ManagerObject::Render()
{
	for (auto& obj : _objectList)
	{
		obj->Render();
	}
}
