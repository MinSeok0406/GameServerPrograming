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

bool RBT_DropNode(RBTNode** root)
{
	RBTNode* node = *root;
	RBTNode* parent = node->parent;

	if (parent == nullptr)
	{
		*root = nullptr;
	}
	else if (parent->left == node)
	{
		parent->left = Nil;
	}
	else
	{
		parent->right = Nil;
	}

	delete node;
	return true;
}

RBTNode* RBT_SearchNode(RBTNode** tree, int target)
{
	RBTNode* node = *tree;

	while (true)
	{
		if (node == Nil)
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
		(*root)->Color = RBTNode::COLOR::BLACK;
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

bool RBT_DeleteNode(RBTNode** tree, int deleteData)
{
	RBTNode* node = RBT_SearchNode(tree, deleteData);
	if (node == nullptr)
	{
		return false;
	}

	RBTNode::COLOR color = node->Color;
	RBTNode* childNode = RBT_DeleteNodeHelper(tree, node);
	RBT_RebuildAfterDelete(tree, childNode, color);

	return true;
}

RBTNode* RBT_DeleteNodeHelper(RBTNode** tree, RBTNode* node)
{
	if (node->left != Nil)
	{
		RBTNode* deleteNode = RBT_SearchMaxNode(node->left);
		node->data = deleteNode->data;
		node->Color = deleteNode->Color;

		RBTNode* parent = deleteNode->parent;
		RBTNode* child = deleteNode->left;
		if (parent->left == deleteNode)
		{
			parent->left = child;
		}
		else
		{
			parent->right = child;
		}

		// Nil 오히려 부모 연결이 필요하지 않을까?
		if (child != Nil)
		{
			child->parent = parent;
		}

		RBT_DropNode(&deleteNode);
		return child;
	}
	else if (node->right != Nil)
	{
		RBTNode* deleteNode = RBT_SearchMinNode(node->right);
		node->data = deleteNode->data;
		node->Color = deleteNode->Color;

		RBTNode* parent = deleteNode->parent;
		RBTNode* child = deleteNode->right;
		if (parent->left == deleteNode)
		{
			parent->left = child;
		}
		else
		{
			parent->right = child;
		}

		// Nil 오히려 부모 연결이 필요하지 않을까?
		if (child != Nil)
		{
			child->parent = parent;
		}

		RBT_DropNode(&deleteNode);
		return child;
	}
	else
	{
		RBT_DropNode(&node);
		return Nil;
	}
}

bool RBT_RebuildAfterDelete(RBTNode** root, RBTNode* x, RBTNode::COLOR color)
{
	if (color == RBTNode::COLOR::RED)
	{
		(*root)->Color = RBTNode::COLOR::BLACK;
		return true;
	}

	while (true)
	{
		if (x == x->parent->left)
		{
			// 2.1 삭제 노드의 자식(기준노드)이 레드인 경우
			if (x->Color == RBTNode::COLOR::RED)
			{
				x->Color = RBTNode::COLOR::BLACK;
				break;
			}
			else
			{
				RBTNode* sibling = x->parent->right;
				// 2.2 삭제 노드의 형제가 레드
				if (sibling->Color == RBTNode::COLOR::RED)
				{
					sibling->Color = RBTNode::COLOR::BLACK;
					RBT_RotateLeft(root, x->parent);
					x->parent->Color = RBTNode::COLOR::RED;
				}
				else 
				{
					// 2.3 삭제 노드의 형제가 블랙이고 형제의 양쪽 자식이 블랙
					if (sibling->left->Color == RBTNode::COLOR::BLACK &&
						sibling->right->Color == RBTNode::COLOR::BLACK)
					{
						sibling->Color = RBTNode::COLOR::RED;
						x = x->parent;
						continue;
					}
					// 2.4 삭제 노드의 형제가 블랙이고 형제의 오른자식은 블랙
					else if (sibling->right->Color == RBTNode::COLOR::BLACK)
					{
						sibling->left->Color = RBTNode::COLOR::BLACK;
						sibling->Color = RBTNode::COLOR::RED;
						RBT_RotateRight(root, sibling);
					}
					
					// 2.5 삭제 노드의 형제가 블랙이고 형제의 오른자식이 레드
					sibling->Color = x->parent->Color;
					x->parent->Color = RBTNode::COLOR::BLACK;
					sibling->right->Color = RBTNode::COLOR::BLACK;
					RBT_RotateLeft(root, x->parent);
					break;
				}
			}
		}
		else
		{
			if (x->Color == RBTNode::COLOR::RED)
			{
				x->Color = RBTNode::COLOR::BLACK;
				break;
			}
			else
			{
				RBTNode* sibling = x->parent->left;
				if (sibling->Color == RBTNode::COLOR::RED)
				{
					sibling->Color = RBTNode::COLOR::BLACK;
					RBT_RotateRight(root, x->parent);
					x->parent->Color = RBTNode::COLOR::RED;
				}
				else
				{
					if (sibling->left->Color == RBTNode::COLOR::BLACK &&
						sibling->right->Color == RBTNode::COLOR::BLACK)
					{
						sibling->Color = RBTNode::COLOR::RED;
						x = x->parent;
						continue;
					}
					else if (sibling->left->Color == RBTNode::COLOR::BLACK)
					{
						sibling->right->Color = RBTNode::COLOR::BLACK;
						sibling->Color = RBTNode::COLOR::RED;
						RBT_RotateLeft(root, sibling);
					}

					sibling->Color = x->parent->Color;
					x->parent->Color = RBTNode::COLOR::BLACK;
					sibling->left->Color = RBTNode::COLOR::BLACK;
					RBT_RotateRight(root, x->parent);
					break;
				}
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

bool Update(RBTNode** root)
{
	return true;
}

bool Render(RBTNode** root)
{
	return true;
}