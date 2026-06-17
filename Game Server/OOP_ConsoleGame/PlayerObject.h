#pragma once
#include "IBaseObject.h"

class PlayerObject : public IBaseObject
{
public:
	PlayerObject(int live, int hp, int x, int y);
	virtual ~PlayerObject();

	bool Update() override;
	void Render() override;

	void movement();
	void attack();
	bool die();

protected:
	bool _live;
	int _hp;
	int _x;
	int _y;
};

struct Player
{
	bool live;
	int hp;
	int x;
	int y;
};

struct playerBullet
{
	int x;
	int y;
};

bool PlayerDie();