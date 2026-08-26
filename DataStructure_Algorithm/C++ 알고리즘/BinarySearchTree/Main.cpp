#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include <vector>
#include "RedBlackTree.h"
using namespace std;

#pragma comment(lib, "winmm.lib")

RBTNode* Nil = new RBTNode;
RBTNode* root = Nil;

vector<int> insertKey;

int wmain()
{
	timeBeginPeriod(1);
	srand((unsigned int)time(nullptr));
	Nil->Color = RBTNode::COLOR::BLACK;

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