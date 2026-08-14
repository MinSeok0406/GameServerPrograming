#include <iostream>
#include <array>
#include <Windows.h>
#include <chrono>
#include <time.h>
#include "ObjectPool.h"
using namespace std;

#pragma comment(lib, "Winmm.lib")

class Object
{
public:
	Object() {}
	virtual ~Object() = default;

private:
	array<int, 10 * 1024 * 1024> _data;
};

using Pool = ObjectPool<Object>;

shared_ptr<Object> getObject(Pool& pool)
{
	auto object = pool.acquireObject();
	return object;
}

void processObject(Object* object)
{
	
}

int wmain()
{
	timeBeginPeriod(1);
	srand((unsigned int)time(nullptr));
	const size_t num = 500000;

	cout << "Starting loop using pool..." << "\n";
	Pool requestPool;
	auto start1 = chrono::steady_clock::now();
	for (auto i = 0; i < num; ++i)
	{
		auto object = getObject(requestPool);
		processObject(object.get());
	}

	auto diff1 = chrono::steady_clock::now() - start1;
	cout << format("{}ms\n", chrono::duration<double, milli>(diff1).count());

	cout << "Starting loop using new/delete..." << "\n";
	auto start2 = chrono::steady_clock::now();
	for (auto i = 0; i < num; ++i)
	{
		auto object = new Object;
		processObject(object);
		delete object;
	}
	auto diff2 = chrono::steady_clock::now() - start2;
	cout << format("{}ms\n", chrono::duration<double, milli>(diff2).count());

	return 0;
}