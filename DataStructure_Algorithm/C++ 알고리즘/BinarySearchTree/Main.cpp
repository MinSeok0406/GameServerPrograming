#include <iostream>
#include "RedBlackTree.h"
using namespace std;

RBTNode* Nil = nullptr;
RBTNode** root = nullptr;

int wmain()
{
	Nil->Color = RBTNode::COLOR::BLACK;
	RBT_InsertNode(root, RBT_CreateNode(40));
	RBT_InsertNode(root, RBT_CreateNode(30));
	RBT_InsertNode(root, RBT_CreateNode(20));
	RBT_InsertNode(root, RBT_CreateNode(10));
	RBT_InsertNode(root, RBT_CreateNode(50));
	RBT_InsertNode(root, RBT_CreateNode(60));

	while (true)
	{
		if (!Update(root))
		{
			break;
		}

		if (!Render(root))
		{
			break;
		}
	}

	return 0;
}