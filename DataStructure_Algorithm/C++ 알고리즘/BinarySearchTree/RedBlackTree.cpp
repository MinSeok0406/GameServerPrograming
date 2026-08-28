#include "RedBlackTree.h"
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <string>

extern RBTNode* Nil;
static int s_level = 0;

bool RBT_VerificationTest(RBTNode** root)
{
	if ((*root) == Nil)
	{
		return true;
	}

	// 0. 루트 노드의 부모 노드 nullptr 검증
	if ((*root)->parent != nullptr)
	{
		printf("root parent error\n");
		return false;
	}

	// 1. 루트 노드 검정색
	if ((*root)->Color != RBTNode::COLOR::BLACK)
	{
		printf("root color error\n");
		return false;
	}

	// 2. Nil이 검정색
	if (Nil->Color != RBTNode::COLOR::BLACK)
	{
		printf("Nil color error\n");
		return false;
	}

	// 3. 데이터 정렬 잘되어 있는지
	std::vector<int> v;
	RBT_InorderTraval(*root, v);
	int a = v[0];
	for (auto i = 1; i < v.size(); ++i)
	{
		if (a > v[i])
		{
			printf("Data Array error\n");
			return false;
		}
		a = v[i];
	}

	// 4. 레드 자식 블랙인지
	if (!RBT_RedChildrenBlack(*root))
	{
		printf("Red Children Not Black\n");
		return false;
	}

	// 5. 루트부터 리프까지 검정색 일정한지
	if (RBT_CheckBlackCount(*root) == -1)
	{
		printf("BlackCount Not equle\n");
		return false;
	}

	// 6. 모든노드의 부모노드 검증
	if (!RBT_ParentLinks(*root, nullptr))
	{
		printf("Parent Not equle\n");
		return false;
	}

	return true;
}

bool RBT_InorderTraval(RBTNode* root, std::vector<int>& v)
{
	if (root == Nil || root == nullptr)
	{
		return true;
	}

	RBT_InorderTraval(root->left, v);
	v.push_back(root->data);
	RBT_InorderTraval(root->right, v);

	return true;
}

bool RBT_RedChildrenBlack(RBTNode* root)
{
	if (root == Nil)
	{
		return true;
	}

	if (root->Color == RBTNode::COLOR::RED)
	{
		if (root->left->Color != RBTNode::COLOR::BLACK || root->right->Color != RBTNode::COLOR::BLACK)
		{
			return false;
		}
	}

	if (!RBT_RedChildrenBlack(root->left))
	{
		return false;
	}

	if (!RBT_RedChildrenBlack(root->right))
	{
		return false;
	}

	return true;
}

int RBT_CheckBlackCount(RBTNode* root)
{
	if (root == Nil)
	{
		return 1;
	}

	int leftCount = RBT_CheckBlackCount(root->left);
	if (leftCount == -1)
	{
		return -1;
	}

	int rightCount = RBT_CheckBlackCount(root->right);
	if (rightCount == -1)
	{
		return -1;
	}

	if (leftCount != rightCount)
	{
		return -1;
	}

	return leftCount + (root->Color == RBTNode::COLOR::BLACK ? 1 : 0);
}

bool RBT_ParentLinks(RBTNode* root, RBTNode* parent)
{
	if (root == Nil)
	{
		return true;
	}

	if (root->parent != parent)
	{
		return false;
	}

	return RBT_ParentLinks(root->left, root) && RBT_ParentLinks(root->right, root);
}

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
	if ((*tree) == nullptr)
	{
		return nullptr;
	}

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
	if (!RBT_InsertNodeHelper(tree, newNode))
	{
		delete newNode;
		return false;
	}

	newNode->Color = RBTNode::COLOR::RED;
	newNode->left = Nil;
	newNode->right = Nil;

	RBT_RebuildAfterInsert(tree, newNode);

	// 검증 테스트 삽입
	/*if (!RBT_VerificationTest(tree))
	{
		__debugbreak();
	}*/

	return true;
}

bool RBT_InsertNodeHelper(RBTNode** tree, RBTNode* newNode)
{
	if (*tree == Nil)
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
		else
		{
			return false;
		}
	}

	return true;
}

bool RBT_RebuildAfterInsert(RBTNode** root, RBTNode* x)
{
	if (x->parent == nullptr)
	{
		(*root) = x;
		(*root)->Color = RBTNode::COLOR::BLACK;
		return true;
	}

	if (x->parent->Color == RBTNode::COLOR::BLACK)
	{
		(*root)->Color = RBTNode::COLOR::BLACK;
		return true;
	}

	while (x != *root && x->parent->Color == RBTNode::COLOR::RED)
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

	RBTNode::COLOR color;
	RBTNode* parent = nullptr;
	RBTNode* childNode = RBT_DeleteNodeHelper(tree, node, color, parent);
	RBT_RebuildAfterDelete(tree, childNode, parent, color);

	// 검증 테스트 삽입
	/*if (!RBT_VerificationTest(tree))
	{
		__debugbreak();
	}*/

	return true;
}

RBTNode* RBT_DeleteNodeHelper(RBTNode** tree, RBTNode* node, RBTNode::COLOR& outColor, RBTNode*& outParent)
{
	if (node->left != Nil)
	{
		RBTNode* deleteNode = RBT_SearchMaxNode(node->left);
		node->data = deleteNode->data;
		outColor = deleteNode->Color;

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

		if (child != Nil)
		{
			child->parent = parent;
		}
		outParent = parent;

		delete deleteNode;
		return child;
	}
	else if (node->right != Nil)
	{
		RBTNode* deleteNode = RBT_SearchMinNode(node->right);
		node->data = deleteNode->data;
		outColor = deleteNode->Color;

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

		if (child != Nil)
		{
			child->parent = parent;
		}
		outParent = parent;

		delete deleteNode;
		return child;
	}
	else
	{
		outColor = node->Color;
		outParent = node->parent;
		if (node->parent == nullptr)
		{
			*tree = Nil;
		}

		RBT_DropNode(&node);
		return Nil;
	}
}

bool RBT_RebuildAfterDelete(RBTNode** root, RBTNode* x, RBTNode* parent, RBTNode::COLOR color)
{
	if (*root == Nil)
	{
		return true;
	}

	if (color == RBTNode::COLOR::RED)
	{
		(*root)->Color = RBTNode::COLOR::BLACK;
		return true;
	}

	while (x != *root && parent != nullptr)
	{
		if (x == parent->left)
		{
			// 2.1 삭제 노드의 자식(기준노드)이 레드인 경우
			if (x->Color == RBTNode::COLOR::RED)
			{
				x->Color = RBTNode::COLOR::BLACK;
				break;
			}
			else
			{
				RBTNode* sibling = parent->right;
				// 2.2 삭제 노드의 형제가 레드
				if (sibling->Color == RBTNode::COLOR::RED)
				{
					sibling->Color = RBTNode::COLOR::BLACK;
					parent->Color = RBTNode::COLOR::RED;
					RBT_RotateLeft(root, parent);
				}
				else 
				{
					// 2.3 삭제 노드의 형제가 블랙이고 형제의 양쪽 자식이 블랙
					if (sibling->left->Color == RBTNode::COLOR::BLACK &&
						sibling->right->Color == RBTNode::COLOR::BLACK)
					{
						sibling->Color = RBTNode::COLOR::RED;
						x = parent;
						parent = parent->parent;
						continue;
					}
					// 2.4 삭제 노드의 형제가 블랙이고 형제의 오른자식은 블랙
					else if (sibling->right->Color == RBTNode::COLOR::BLACK)
					{
						sibling->left->Color = RBTNode::COLOR::BLACK;
						sibling->Color = RBTNode::COLOR::RED;
						RBT_RotateRight(root, sibling);
						sibling = parent->right;
					}
					
					// 2.5 삭제 노드의 형제가 블랙이고 형제의 오른자식이 레드
					sibling->Color = parent->Color;
					parent->Color = RBTNode::COLOR::BLACK;
					sibling->right->Color = RBTNode::COLOR::BLACK;
					RBT_RotateLeft(root, parent);
					break;
				}
			}
		}
		else
		{
			// 2.1 삭제 노드의 자식(기준노드)이 레드인 경우
			if (x->Color == RBTNode::COLOR::RED)
			{
				x->Color = RBTNode::COLOR::BLACK;
				break;
			}
			else
			{
				RBTNode* sibling = parent->left;
				// 2.2 삭제 노드의 형제가 레드
				if (sibling->Color == RBTNode::COLOR::RED)
				{
					sibling->Color = RBTNode::COLOR::BLACK;
					parent->Color = RBTNode::COLOR::RED;
					RBT_RotateRight(root, parent);
				}
				else
				{
					// 2.3 삭제 노드의 형제가 블랙이고 형제의 양쪽 자식이 블랙
					if (sibling->left->Color == RBTNode::COLOR::BLACK &&
						sibling->right->Color == RBTNode::COLOR::BLACK)
					{
						sibling->Color = RBTNode::COLOR::RED;
						x = parent;
						parent = parent->parent;
						continue;
					}
					// 2.4 삭제 노드의 형제가 블랙이고 형제의 오른자식은 블랙
					else if (sibling->left->Color == RBTNode::COLOR::BLACK)
					{
						sibling->right->Color = RBTNode::COLOR::BLACK;
						sibling->Color = RBTNode::COLOR::RED;
						RBT_RotateLeft(root, sibling);
						sibling = parent->left;
					}

					// 2.5 삭제 노드의 형제가 블랙이고 형제의 오른자식이 레드
					sibling->Color = parent->Color;
					parent->Color = RBTNode::COLOR::BLACK;
					sibling->left->Color = RBTNode::COLOR::BLACK;
					RBT_RotateRight(root, parent);
					break;
				}
			}
		}
	}

	if (*root != Nil)
	{
		(*root)->Color = RBTNode::COLOR::BLACK;
	}
	return true;
}

bool RBT_RotateLeft(RBTNode** root, RBTNode* parent)
{
	RBTNode* rightChild = parent->right;

	parent->right = rightChild->left;

	if (rightChild->left != Nil)
	{
		rightChild->left->parent = parent;
	}

	rightChild->parent = parent->parent;

	if (parent->parent == nullptr)
	{
		(*root) = rightChild;
	}
	else
	{
		if (parent == parent->parent->left)
		{
			parent->parent->left = rightChild;
		}
		else if (parent == parent->parent->right)
		{
			parent->parent->right = rightChild;
		}
	}

	rightChild->left = parent;
	parent->parent = rightChild;

	return true;
}

bool RBT_RotateRight(RBTNode** root, RBTNode* parent)
{
	RBTNode* leftChild = parent->left;

	parent->left = leftChild->right;

	if (leftChild->right != Nil)
	{
		leftChild->right->parent = parent;
	}

	leftChild->parent = parent->parent;

	if (parent->parent == nullptr)
	{
		(*root) = leftChild;
	}
	else
	{
		if (parent == parent->parent->left)
		{
			parent->parent->left = leftChild;
		}
		else if (parent == parent->parent->right)
		{
			parent->parent->right = leftChild;
		}
	}

	leftChild->right = parent;
	parent->parent = leftChild;

	return true;
}

bool Update(RBTNode** root)
{
	int num = 0;
	printf("1. Insert Node \t");
	printf("2. Delete Node \t");
	printf("3. Search Node \t");
	printf("4. exit \n");
	printf("Select : ");
	scanf_s("%d", &num);

	switch (num)
	{
	case 1:
		if (!Insert(root))
		{
			printf("insert fail!!\n\n");
		}
		break;
	case 2:
		if (!Delete(root))
		{
			printf("delete fail!!\n\n");
		}
		break;
	case 3:
		if (!Search(root))
		{
			printf("search fail!!\n\n");
		}
		break;
	case 4:
		return false;
		break;
	}

	printf("\n\n");
	return true;
}

bool Insert(RBTNode** root)
{
	int num = 0;
	printf("Insert Data : ");
	scanf_s("%d", &num);

	if (!RBT_InsertNode(root, RBT_CreateNode(num)))
	{
		return false;
	}

	return true;
}

bool Delete(RBTNode** root)
{
	int num = 0;
	printf("Delete Data : ");
	scanf_s("%d", &num);

	if (!RBT_DeleteNode(root, num))
	{
		return false;
	}

	return true;
}

bool Search(RBTNode** root)
{
	int num = 0;
	printf("Search Data : ");
	scanf_s("%d", &num);

	if (!RBT_SearchNode(root, num))
	{
		return false;
	}

	printf("Find %d!!\n", num);
	return true;
}

static int getTreeHeight(RBTNode* node)
{
	if (node == Nil)
	{
		return 0;
	}

	int leftHeight = getTreeHeight(node->left);
	int rightHeight = getTreeHeight(node->right);
	return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

static void setNodeColor(RBTNode::COLOR color)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (color == RBTNode::COLOR::RED)
	{
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
	}
	else
	{
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
}

static void resetConsoleColor()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

static void printNode(RBTNode* node)
{
	char buffer[16];
	if (node == Nil)
	{
		setNodeColor(RBTNode::COLOR::BLACK);
		memcpy_s(buffer, sizeof(buffer), "N", sizeof("N"));
	}
	else
	{
		setNodeColor(node->Color);
		sprintf_s(buffer, "%d", node->data);
	}
	printf("%s", buffer);
	resetConsoleColor();
}

bool Render(RBTNode** root)
{
	if (*root == Nil)
	{
		return true;
	}

	int height = getTreeHeight(*root);

	for (auto level = 0; level <= height; ++level)
	{
		int depth = (1 << (height - level)) - 1;
		s_level = (1 << (height - level + 1)) - 1;

		printf("%s", std::string(depth, ' ').c_str());
		BFS(root, level);
		printf("\n");
	}

	printf("\n");
	return true;
}

bool BFS(RBTNode** root, int depth)
{
	RBTNode* node = *root;

	if (depth == 0)
	{
		printNode(node);
		printf("%s", std::string(s_level, ' ').c_str());
		return true;
	}

	RBTNode* leftChild = (node == Nil) ? Nil : node->left;
	RBTNode* rightChild = (node == Nil) ? Nil : node->right;

	BFS(&leftChild, depth - 1);
	BFS(&rightChild, depth - 1);

	return true;
}
