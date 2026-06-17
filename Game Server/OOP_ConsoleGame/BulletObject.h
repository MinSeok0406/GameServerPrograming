#pragma once
#include "IBaseObject.h"

class BulletObject : public IBaseObject
{
public:
	BulletObject(int x, int y, OBJECT_TYPE objType);
	virtual ~BulletObject();

	bool Update() override;
	void Render() override;

protected:
	int _x;
	int _y;
};