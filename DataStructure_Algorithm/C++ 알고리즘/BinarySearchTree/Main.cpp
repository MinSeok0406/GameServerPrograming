#include <iostream>
#include "RedBlackTree.h"
using namespace std;

RBTNode* Nil = new RBTNode;
RBTNode* root = nullptr;

int wmain()
{
	Nil->Color = RBTNode::COLOR::BLACK;

	while (true)
	{
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