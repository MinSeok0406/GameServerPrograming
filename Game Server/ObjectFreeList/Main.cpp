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
	Player() { printf("Player\n"); }
	~Player() { printf("~Player\n"); }
};

class Monster : public Object
{
public:
	Monster() { printf("Monster\n"); }
	~Monster() { printf("~Monster\n"); }
};

const size_t num = 500000;
ObjectFreeList<Player> g_playerPool(10, true);

int wmain()
{
	timeBeginPeriod(1);
	srand((unsigned int)time(nullptr));

	Player* player = g_playerPool.Alloc();
	g_playerPool.Free(player);
	
	return 0;
}