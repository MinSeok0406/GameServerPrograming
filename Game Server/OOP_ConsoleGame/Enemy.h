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

struct enemyBullet
{
	int x;
	int y;
};

void EnemyInit();
void EnemyMovement();
void EnemyAttack();
void EnemyRendering();
bool EnemyDie();