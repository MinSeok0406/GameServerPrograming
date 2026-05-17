#include <iostream>
#include <Windows.h>
#include <vector>
#include "Player.h"
#include "Enemy.h"
#include "Console.h"
#include "Buffer.h"
using namespace std;

extern Player* p;
extern vector<pair<int, int>> Playerbullet;

vector<Enemy> enemies;
vector<pair<int, int>> Enemybullet;
bool isMove = true;

void PlayerInit()
{
	// 텍스트 파일 데이터 불러오기
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

	Sprite_Draw(p->x, p->y, 'A');
}

void PlayerAttack()
{
	// 총알 생성
	if (GetAsyncKeyState(VK_SPACE))
	{
		Playerbullet.push_back({ p->x, p->y - 1});
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

			y = Playerbullet[i].second - 1;
			x = Playerbullet[i].first;
			if (enemies[j].x == x && enemies[j].y == y)
			{
				Playerbullet[i].second = -1;
				Playerbullet[i].first = -1;
				enemies[j].live = false;
			}
		}
	}

	// 총알 이동
	for (int i = 0; i < (int)Playerbullet.size(); ++i)
	{
		if (Playerbullet[i].first != -1 && Playerbullet[i].second != -1)
		{
			Sprite_Draw(Playerbullet[i].first, Playerbullet[i].second, '^');
			Playerbullet[i].second -= 1;

			if (Playerbullet[i].second < 0)
			{
				Playerbullet[i].second = -1;
				Playerbullet[i].first = -1;
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