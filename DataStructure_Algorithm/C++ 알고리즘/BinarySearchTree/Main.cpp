#include <iostream>
#include "BinaryTree.h"
#include "RedBlackTree.h"
using namespace std;

RBTNode* Nil = nullptr;
BTree* root = nullptr;

int wmain()
{
	while (true)
	{
		if (!Update(&root))
		{
			break;
		}
	}

	return 0;
}