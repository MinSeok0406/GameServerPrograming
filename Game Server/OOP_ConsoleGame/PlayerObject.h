#pragma once
#include "IBaseObject.h"

class PlayerObject : public IBaseObject
{
public:
	PlayerObject(int live, int hp, int x, int y);
	virtual ~PlayerObject();

	bool Update() override;
	void Render() override;
	bool RemoveObject() override;
	bool OnCollision(IBaseObject* obj) override;

	void movement();
	void attack();

	int GetX() override
	{
		return this->_x;
	}

	int GetY() override
	{
		return this->_y;
	}

protected:
	int _hp;
};