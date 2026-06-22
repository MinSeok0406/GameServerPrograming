#include "ManagerObject.h"
#include "PlayerObject.h"
#include "EnemyObject.h"
#include "BulletObject.h"
#include "Profile.h"

#ifdef PROFILE
#define PRO_BEGIN(TagName)	ProfileBegin(TagName)
#define PRO_END(TagName)	ProfileEnd(TagName)
#else
#define PRO_BEGIN(TagName)	OUT
#define PRO_END(TagName)	OUT
#endif

ManagerObject* ManagerObject::_pManagerObject = nullptr;

// 플레이어 오브젝트 생성 함수
void ManagerObject::CreateObject(OBJECT_TYPE objType)
{
	PRO_BEGIN(L"CreateObject");
	IBaseObject* ibo;

	if (objType == OBJECT_TYPE::PLAYER)
	{
		ibo = new PlayerObject(true, 100, 40, 20);
	}

	_tempList.push_back(ibo);
	PRO_END(L"CreateObject");
}

// 적 오브젝트 생성 함수
void ManagerObject::CreateObject(OBJECT_TYPE objType, bool live, bool isMove, char sprite, int x, int y, int moveX, int moveY)
{
	PRO_BEGIN(L"CreateObject");

	IBaseObject* ibo;

	if (objType == OBJECT_TYPE::ENEMY)
	{
		ibo = new EnemyObject(live, isMove, sprite, x, y, moveX, moveY);
	}

	_tempList.push_back(ibo);
	PRO_END(L"CreateObject");
}

// 위치가 필요한 오브젝트 생성 함수
void ManagerObject::CreateObject(OBJECT_TYPE objType, int x, int y)
{
	PRO_BEGIN(L"CreateObject");

	IBaseObject* ibo;

	if (objType == OBJECT_TYPE::PLAYER_BULLET)
	{
		// 플레이어 쪽에서 위치 줘야됨
		ibo = new BulletObject(OBJECT_TYPE::PLAYER_BULLET, x, y, true);
	}
	else if (objType == OBJECT_TYPE::ENEMY_BULLET)
	{
		// 적 쪽에서 위치 줘야됨
		ibo = new BulletObject(OBJECT_TYPE::ENEMY_BULLET, x, y, true);
	}

	_tempList.push_back(ibo);
	PRO_END(L"CreateObject");
}

bool ManagerObject::Update()
{
	PRO_BEGIN(L"Update");

	for (auto iter = _objectList.cbegin(); iter != _objectList.cend(); ++iter)
	{
		auto pObject = *iter;
		if (!pObject->RemoveObject())
		{
			continue;
		}
		pObject->Update();

		// 충돌 처리
		auto targetIter = iter;
		for (++targetIter; targetIter != _objectList.cend(); ++targetIter)
		{
			auto pTargetObject = *targetIter;
			if (!pTargetObject->RemoveObject())
			{
				continue;
			}

			if (pObject->GetX() == pTargetObject->GetX()
				&& pObject->GetY() == pTargetObject->GetY())
			{
				pObject->OnCollision(pTargetObject);
				pTargetObject->OnCollision(pObject);
			}
		}
	}

	// 추가된 오브젝트 리스트에 추가
	if (_tempList.empty() == false)
	{
		_objectList.splice(_objectList.cend(), _tempList);
	}

	PRO_END(L"Update");
	return true;
}

void ManagerObject::Render()
{
	PRO_BEGIN(L"Render");
	for (auto& obj : _objectList)
	{
		if (!obj->RemoveObject())
		{
			continue;
		}

		obj->Render();
	}
	PRO_END(L"Render");
}

void ManagerObject::RemoveObject()
{
	PRO_BEGIN(L"RemoveObject");

	for (auto iter = _objectList.cbegin(); iter != _objectList.cend();)
	{
		auto obj = *iter;
		if (obj == nullptr)
		{
			iter = _objectList.erase(iter);
			continue;
		}

		if (obj->RemoveObject())
		{
			delete obj;
			iter = _objectList.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	PRO_END(L"RemoveObject");
}