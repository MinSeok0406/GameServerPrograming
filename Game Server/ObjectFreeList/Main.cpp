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

int wmain()
{
	timeBeginPeriod(1);
	srand((unsigned int)time(nullptr));

	ObjectFreeList<Player> g_playerPool(50, true);

	// 객체에게 포인터 주소를 넘겨줘야 되는데, 그 주소 값 자체를 넘겨주고있다.
	// 이건 아닌데...
	Player* player = g_playerPool.Alloc();
	
	return 0;
}