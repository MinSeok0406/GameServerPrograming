#pragma once
#include "IBaseObject.h"

class EnemyObject : public IBaseObject
{
public:
	EnemyObject(bool live, bool isMove, char sprite,
		int x, int y, int moveX, int moveY);
	virtual ~EnemyObject();

	bool Update() override;
	void Render() override;

	void movement();
	void attack();
	bool die();

protected:
	bool _isMove;
	bool _live;
	char _sprite;
	int _x;
	int _y;
	int _moveX;
	int _moveY;
};

struct Enemy
{
	bool live;
	char sprite;
	int x;
	int y;
	int moveX;
	int moveY;
};

struct enemyBullet
{
	int x;
	int y;
};

void EnemyInit();
bool EnemyDie();