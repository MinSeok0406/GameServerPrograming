#include <iostream>
#include <Windows.h>
#include <vector>
#include "Player.h"
#include "Enemy.h"
#include "Console.h"
#include "Buffer.h"
using namespace std;

Player* p;
vector<playerBullet> Playerbullet;

extern vector<Enemy> enemies;
extern vector<enemyBullet> Enemybullet;
extern bool isMove;

void PlayerInit()
{
	p = new Player{ true, 100, 40, 20 };
}

void PlayerMoveMent()
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
}

void PlayerAttack()
{
	// 총알 생성
	if (GetAsyncKeyState(VK_SPACE))
	{
		Playerbullet.push_back({ p->x, p->y - 1 });
	}

	// 몬스터 피격 판정
	int y = 0;
	int x = 0;
	for (int i = 0; i < (int)Playerbullet.size(); ++i)
	{
		for (int j = 0; j < (int)enemies.size(); ++j)
		{
			if (!enemies[j].live)
			{
				continue;
			}

			y = Playerbullet[i].y - 1;
			x = Playerbullet[i].x;
			if (enemies[j].x == x && enemies[j].y == y)
			{
				Playerbullet[i].y = -1;
				Playerbullet[i].x = -1;
				enemies[j].live = false;
			}
		}
	}
}

void PlayerRendering()
{
	// 플레이어 이동
	Sprite_Draw(p->x, p->y, 'A');

	// 총알 이동
	for (int i = 0; i < (int)Playerbullet.size(); ++i)
	{
		if (Playerbullet[i].x != -1 && Playerbullet[i].y != -1)
		{
			Sprite_Draw(Playerbullet[i].x, Playerbullet[i].y, '^');
			Playerbullet[i].y -= 1;

			if (Playerbullet[i].y < 0)
			{
				Playerbullet[i].y = -1;
				Playerbullet[i].x = -1;
			}
		}
	}
}

bool PlayerDie()
{
	if (p->live)
	{
		return false;
	}

	enemies.clear();
	Enemybullet.clear();
	isMove = true;
	Playerbullet.clear();
	delete p;
	return true;
}