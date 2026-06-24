#pragma once
#include "IBaseObject.h"

class BulletObject : public IBaseObject
{
public:
	BulletObject(OBJECT_TYPE objType, int x, int y, bool live);
	virtual ~BulletObject();

	bool Update() override;
	void Render() override;
	bool RemoveObject() override;
	bool OnCollision(IBaseObject* obj) override;
	int GetX() override
	{
		return this->_x;
	}

	int GetY() override
	{
		return this->_y;
	}
};