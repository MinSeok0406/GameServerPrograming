#pragma once

struct Player
{
	bool live;
	int hp;
	int x;
	int y;
};

void PlayerInit();
void MoveMent();
void Attack();
bool Die();