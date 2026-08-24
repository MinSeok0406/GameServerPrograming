#define _CRT_SECURE_NO_WARNINGS
#include "RedBlackTree.h"
#include <iostream>
#include <stdlib.h>

extern RBTNode* Nil;

RBTNode* RBT_CreateNode(int newData)
{
	RBTNode* newNode = new RBTNode;
	newNode->Color = RBTNode::COLOR::BLACK;
	newNode->parent = nullptr;
	newNode->left = Nil;
	newNode->right = Nil;
	newNode->data = newData;
	return newNode;
}

RBTNode* RBT_SearchNode(RBTNode* tree, int target)
{
	RBTNode* node = tree;

	while (true)
	{
		if (node == nullptr)
		{
			return nullptr;
		}

		if (node->data == target)
		{
			break;
		}
		else if (node->data < target)
		{
			node = node->right;
		}
		else if (node->data > target)
		{
			node = node->left;
		}
	}

	return node;
}

RBTNode* RBT_SearchMinNode(RBTNode* tree)
{
	RBTNode* node = tree;
	while (true)
	{
		if (node->left == Nil)
		{
			break;
		}

		node = node->left;
	}

	return node;
}

RBTNode* RBT_SearchMaxNode(RBTNode* tree)
{
	RBTNode* node = tree;
	while (true)
	{
		if (node->right == Nil)
		{
			break;
		}

		node = node->right;
	}

	return node;
}

bool RBT_InsertNode(RBTNode** tree, RBTNode* newNode)
{
	RBT_InsertNodeHelper(tree, newNode);

	newNode->Color = RBTNode::COLOR::RED;
	newNode->left = Nil;
	newNode->right = Nil;

	RBT_RebuildAfterInsert(tree, newNode);

	return true;
}

bool RBT_InsertNodeHelper(RBTNode** tree, RBTNode* newNode)
{
	if (*tree == nullptr)
	{
		*tree = newNode;
		return true;
	}

	RBTNode* node = *tree;
	while (true)
	{
		if (node->data > newNode->data)
		{
			if (node->left == Nil)
			{
				node->left = newNode;
				newNode->parent = node;
				break;
			}
			node = node->left;
		}
		else if (node->data < newNode->data)
		{
			if (node->right == Nil)
			{
				node->right = newNode;
				newNode->parent = node;
				break;
			}
			node = node->right;
		}
	}

	return true;
}

bool RBT_RebuildAfterInsert(RBTNode** root, RBTNode* x)
{
	if (x->parent->Color == RBTNode::COLOR::BLACK)
	{
		return true;
	}

	while (x != *root)
	{
		if (x->parent == x->parent->parent->left)
		{
			RBTNode* uncle = x->parent->parent->right;
			if (uncle->Color == RBTNode::COLOR::RED)
			{
				x->parent->Color = RBTNode::COLOR::BLACK;
				uncle->Color = RBTNode::COLOR::BLACK;
				x->parent->parent->Color = RBTNode::COLOR::RED;
				x = x->parent->parent;
			}
			else
			{
				if (x == x->parent->right)
				{
					x = x->parent;
					RBT_RotateLeft(root, x);
				}

				x->parent->Color = RBTNode::COLOR::BLACK;
				x->parent->parent->Color = RBTNode::COLOR::RED;
				RBT_RotateRight(root, x->parent->parent);
			}
		}
		else
		{
			RBTNode* uncle = x->parent->parent->left;
			if (uncle->Color == RBTNode::COLOR::RED)
			{
				x->parent->Color = RBTNode::COLOR::BLACK;
				uncle->Color = RBTNode::COLOR::BLACK;
				x->parent->parent->Color = RBTNode::COLOR::RED;
				x = x->parent->parent;
			}
			else
			{
				if (x == x->parent->left)
				{
					x = x->parent;
					RBT_RotateRight(root, x);
				}

				x->parent->Color = RBTNode::COLOR::BLACK;
				x->parent->parent->Color = RBTNode::COLOR::RED;
				RBT_RotateLeft(root, x->parent->parent);
			}
		}
	}

	(*root)->Color = RBTNode::COLOR::BLACK;

	return true;
}

bool RBT_RotateLeft(RBTNode** root, RBTNode* parent)
{
	RBTNode* rightChild = parent->right;

	if (rightChild->left != Nil)
	{
		rightChild->left->parent = parent;
	}

	parent->right = rightChild->left;

	if (parent->parent == nullptr)
	{
		(*root) = rightChild;
	}
	else
	{
		if (parent == parent->parent->left)
		{
			rightChild = parent->parent->left;
		}
		else if (parent == parent->parent->right)
		{
			rightChild = parent->parent->right;
		}
	}

	rightChild->left = parent;
	parent->parent = rightChild->parent;

	return true;
}

bool RBT_RotateRight(RBTNode** root, RBTNode* parent)
{
	RBTNode* leftChild = parent->left;

	if (leftChild->right != Nil)
	{
		leftChild->right->parent = parent;
	}

	parent->left = leftChild->right;

	if (parent->parent == nullptr)
	{
		(*root) = leftChild;
	}
	else
	{
		if (parent == parent->parent->left)
		{
			leftChild = parent->parent->left;
		}
		else if (parent == parent->parent->right)
		{
			leftChild = parent->parent->right;
		}
	}

	leftChild->right = parent;
	parent->parent = leftChild->parent;

	return true;
}