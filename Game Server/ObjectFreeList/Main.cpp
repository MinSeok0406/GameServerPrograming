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
};

class Monster : public Object
{
public:
	Monster() {}
	~Monster() {}
};

const size_t num = 500000;
ObjectFreeList<Player> g_playerPool(num, true);
ObjectFreeList<Monster> g_monsterPool(num, true);

int wmain()
{
	timeBeginPeriod(1);
	srand((unsigned int)time(nullptr));

	Object* player = g_playerPool.Alloc();
	Object* monster = g_monsterPool.Alloc();

	//g_playerPool.Free(player);
	//g_monsterPool.Free(monster);

	return 0;
}