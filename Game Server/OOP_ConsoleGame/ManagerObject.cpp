#include "ManagerObject.h"
#include "PlayerObject.h"
#include "EnemyObject.h"
#include "BulletObject.h"

ManagerObject* ManagerObject::_pManagerObject = nullptr;

// 플레이어 오브젝트 생성 함수
void ManagerObject::CreateObject(OBJECT_TYPE objType)
{
	IBaseObject* ibo;

	if (objType == OBJECT_TYPE::PLAYER)
	{
		ibo = new PlayerObject(true, 100, 40, 20);
	}

	_objectList.push_back(ibo);
}

// 적 오브젝트 생성 함수
void ManagerObject::CreateObject(OBJECT_TYPE objType, bool live, bool isMove, char sprite, int x, int y, int moveX, int moveY)
{
	IBaseObject* ibo;

	if (objType == OBJECT_TYPE::ENEMY)
	{
		ibo = new EnemyObject(live, isMove, sprite, x, y, moveX, moveY);
	}

	_objectList.push_back(ibo);
}

// 위치가 필요한 오브젝트 생성 함수
void ManagerObject::CreateObject(OBJECT_TYPE objType, int x, int y)
{
	IBaseObject* ibo;

	if (objType == OBJECT_TYPE::PLAYER_BULLET)
	{
		// 플레이어 쪽에서 위치 줘야됨
		ibo = new BulletObject(x, y, OBJECT_TYPE::PLAYER_BULLET);
	}
	else if (objType == OBJECT_TYPE::ENEMY_BULLET)
	{
		// 적 쪽에서 위치 줘야됨
		ibo = new BulletObject(x, y, OBJECT_TYPE::ENEMY_BULLET);
	}

	_objectList.push_back(ibo);
}

void ManagerObject::DestoryObject(IBaseObject* obj)
{
	std::erase(_objectList, obj);
	delete obj;
}

bool ManagerObject::Update()
{
	auto iter = _objectList.begin();

	for (; iter != _objectList.end(); ++iter)
	{
		auto pObject = *iter;
		pObject->Update();

		// 충돌 처리
		auto targetIter = iter;
		for (++targetIter; targetIter != _objectList.end(); ++targetIter)
		{
			auto pTargetObject = *targetIter;
			if (pObject->GetX() == pTargetObject->GetX()
				&& pObject->GetY() == pTargetObject->GetY())
			{
				pObject->OnCollision(pTargetObject);
				pTargetObject->OnCollision(pObject);
			}
		}
	}

	return true;
}

void ManagerObject::Render()
{
	for (auto& obj : _objectList)
	{
		obj->Render();
	}
}

void ManagerObject::RemoveObject()
{
	for (auto& obj : _objectList)
	{
		obj->RemoveObject();
	}
}