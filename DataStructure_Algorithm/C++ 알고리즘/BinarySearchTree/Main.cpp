#include <iostream>
#include "BinaryTree.h"
using namespace std;

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