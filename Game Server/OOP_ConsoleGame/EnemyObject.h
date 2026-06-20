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
	bool RemoveObject() override;
	bool OnCollision(IBaseObject* obj) override;

	void movement();
	void attack();
	bool die();

protected:
	bool _live;
	bool _isMove;
	char _sprite;
	int _moveX;
	int _moveY;
};