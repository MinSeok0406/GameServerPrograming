#include <iostream>
#include <array>
#include <Windows.h>
#include <chrono>
#include <time.h>
#include "ObjectFreeList.h"
using namespace std;

#pragma comment(lib, "Winmm.lib")

class Object
{
public:
	Object() 
	{
		hp = 100;
		id = 1;
		x = 0;
		y = 0;
	};

	Object(int hp, int id, int x, int y)
		: hp(hp), id(id), x(x), y(y)
	{
	}

	~Object() 
	{ 
	};

private:
	int hp;
	int id;
	short x;
	short y;
};

class Player : public Object
{
public:
	Player() {}
	~Player() {}

	
private:
	long long _type;
};

class Monster : public Object
{
public:
	Monster() {}
	~Monster() {}

	
private:
	long long _type;
};

const size_t num = 500000;
ObjectFreeList<Player> g_playerPool(50, true);
ObjectFreeList<Monster> g_monsterPool(50, true);
vector<Player*> v;

int wmain()
{
	timeBeginPeriod(1);
	srand((unsigned int)time(nullptr));

	Player* p = nullptr;
	while (true)
	{
		int randNum = rand() % 10;
		if (randNum < 5)
		{
			p = g_playerPool.Alloc();
			v.emplace_back(p);
		}
		else
		{
			if (v.empty())
			{
				continue;
			}

			g_playerPool.Free(v.back());
			v.pop_back();
		}
	}

	return 0;
}