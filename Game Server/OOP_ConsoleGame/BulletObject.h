#pragma once
#include "IBaseObject.h"

class BulletObject : public IBaseObject
{
public:
	BulletObject(int x, int y, OBJECT_TYPE objType);
	virtual ~BulletObject();

	bool Update() override;
	void Render() override;
	bool RemoveObject() override;
	bool OnCollision(IBaseObject* obj) override;

protected:
	bool _live;
};