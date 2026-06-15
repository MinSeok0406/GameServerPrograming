#pragma once

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

void PlayerInit();
void PlayerMoveMent();
void PlayerAttack();
void PlayerRendering();
bool PlayerDie();