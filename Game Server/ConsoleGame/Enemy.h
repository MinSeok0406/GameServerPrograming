#pragma once

struct Enemy
{
	bool live;
	char sprite;
	int x;
	int y;
	int moveX;
	int moveY;
};

void EnemyInit();
void EnemyMovement();
void EnemyAttack();
bool EnemyDie();