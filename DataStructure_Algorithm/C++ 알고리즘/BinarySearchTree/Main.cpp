#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <chrono>
#include "RedBlackTree.h"
using namespace std;

#pragma comment(lib, "winmm.lib")

RBTNode* Nil = new RBTNode;
RBTNode* root = Nil;

map<int, int> mp;

vector<int> insertKey;

int wmain()
{
	timeBeginPeriod(1);
	srand((unsigned int)time(nullptr));
	Nil->Color = RBTNode::COLOR::BLACK;

	// 성능 비교 (map)
	// 레드블랙트리 -> 57000ms
/*	auto start = chrono::steady_clock::now();

	for (auto i = 0; i < 100'000'000; ++i)
	{
		RBT_InsertNode(&root, RBT_CreateNode(i));
	}

	for (auto i = 0; i < 100'000'000; ++i)
	{
		RBT_DeleteNode(&root, i);
	}

	auto end = chrono::steady_clock::now();

	chrono::duration<double, milli> duration_ms = end - start;
	printf("%lf\n",duration_ms.count());


	// map -> 75000ms
	start = chrono::steady_clock::now();

	for (auto i = 0; i < 100'000'000; ++i)
	{
		mp[i] = i;
	}

	for (auto i = 0; i < 100'000'000; ++i)
	{
		mp.erase(i);
	}

	end = chrono::steady_clock::now();

	duration_ms = end - start;
	printf("%lf\n", duration_ms.count());*/

	while (true)
	{
		// 검증 테스트 코드
		/*int randNum = rand() % 100;
		if (randNum < 50 || insertKey.empty())
		{
			int inputData = rand() % 10000;
			RBT_InsertNode(&root, RBT_CreateNode(inputData));
			insertKey.push_back(inputData);
		}
		else
		{
			int idx = rand() % insertKey.size();
			RBT_DeleteNode(&root, insertKey[idx]);
			insertKey.erase(insertKey.begin() + idx);
		}*/

		if (!Update(&root))
		{
			break;
		}

		if (!Render(&root))
		{
			break;
		}
	}

	delete Nil;
	return 0;
}