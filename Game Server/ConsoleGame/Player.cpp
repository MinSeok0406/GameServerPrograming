#include <iostream>
#include <Windows.h>
#include <vector>
#include "Player.h"
#include "Console.h"
#include "Buffer.h"
#include "Enemy.h"
using namespace std;

extern Player* p;
vector<pair<int, int>> bullet;

int tick;

void PlayerInit()
{
	p = new Player{ true, 100, 40, 20 };

	// 텍스트 파일 데이터 불러오기
	Sprite_Draw(p->x, p->y, 'V');
}

void MoveMent()
{
	if (GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_UP))
	{
		p->x -= 1;
		p->y -= 1;
	}
	else if (GetAsyncKeyState(VK_UP) && GetAsyncKeyState(VK_RIGHT))
	{
		p->x += 1;
		p->y -= 1;
	}
	else if (GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_DOWN))
	{
		p->x += 1;
		p->y += 1;
	}
	else if (GetAsyncKeyState(VK_DOWN) && GetAsyncKeyState(VK_LEFT))
	{
		p->x -= 1;
		p->y += 1;
	}
	else if (GetAsyncKeyState(VK_LEFT))
	{
		p->x -= 1;
	}
	else if (GetAsyncKeyState(VK_UP))
	{
		p->y -= 1;
	}
	else if (GetAsyncKeyState(VK_RIGHT))
	{
		p->x += 1;
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		p->y += 1;
	}

	Sprite_Draw(p->x, p->y, 'V');
}

void Attack()
{
	if (GetAsyncKeyState(VK_SPACE))
	{
		bullet.push_back({ p->x, p->y - 1});
	}

	for (unsigned int i = 0; i < bullet.size(); ++i)
	{
		Sprite_Draw(bullet[i].first, bullet[i].second, '^');
		bullet[i].second -= 1;
	}
}

bool Die()
{
	if (p->live)
	{
		return false;
	}

	return true;
}