#pragma once
#include "IBaseObject.h"
#include <list>

class ManagerObject
{
public:
	void CreateObject(OBJECT_TYPE objType);
	void DestoryObject(IBaseObject* obj);

	bool Update();
	void Render();

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

private:
	static ManagerObject* _pManagerObject;

	ManagerObject() = default;
	~ManagerObject() = default;
};