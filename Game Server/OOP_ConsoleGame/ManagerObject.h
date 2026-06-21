#pragma once
#include "IBaseObject.h"
#include <list>

class ManagerObject
{
public:
	void CreateObject(OBJECT_TYPE objType);
	void CreateObject(OBJECT_TYPE objType, int x, int y);
	void CreateObject(OBJECT_TYPE objType, bool live, bool isMove,
		char sprite, int x, int y, int moveX, int moveY);

	bool Update();
	void Render();
	void RemoveObject();

	static ManagerObject* getInstance()
	{
		if (_pManagerObject == nullptr)
		{
			_pManagerObject = new ManagerObject;
		}

		return _pManagerObject;
	}

	static void destoryInstance()
	{
		if (_pManagerObject != nullptr)
		{
			delete _pManagerObject;
			_pManagerObject = nullptr;
		}
	}

	std::list<IBaseObject*> _objectList;
	std::list<IBaseObject*> _tempList;
private:
	static ManagerObject* _pManagerObject;

	ManagerObject() = default;
	~ManagerObject() = default;
};