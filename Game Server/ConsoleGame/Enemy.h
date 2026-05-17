#pragma once

struct Enemy
{
	bool live;
	int x;
	int y;
};

void EnemyInit();
void EnemyMovement();
void EnemyAttack();
bool EnemyDie();