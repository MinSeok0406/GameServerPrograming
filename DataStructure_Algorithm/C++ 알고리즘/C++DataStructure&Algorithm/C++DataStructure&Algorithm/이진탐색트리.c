/*#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef int element;
typedef struct TreeNode
{
	element key;
	struct TreeNode* left, * right;
} TreeNode;

TreeNode* search(TreeNode* node, int key)
{
	if (node == NULL)
		return NULL;

	if (key == node->key)
	{
		return node;
	}
	else if (key < node->key)
	{
		return search(node->left, key);
	}
	else
	{
		return search(node->right, key);
	}
}

TreeNode* new_node(int item)
{
	TreeNode* temp = (TreeNode*)malloc(sizeof(TreeNode));
	temp->key = item;
	temp->left = temp->right = NULL;
	return temp;
}

TreeNode* insert_node(TreeNode* node, int key)
{
	if (node == NULL)
		return new_node(key);

	if (key < node->key)
	{
		node->left = insert_node(node->left, key);
	}
	else if (key > node->key)
	{
		node->right = insert_node(node->right, key);
	}

	return node;
}

TreeNode* min_value_node(TreeNode* node)
{
	TreeNode* current = node;

	while (current->left != NULL)
		current = current->left;

	return current;
}

TreeNode* delete_node(TreeNode* root, int key)
{
	if (root == NULL)
		return root;

	// 찾는 키가 루트보다 작으면
	if (key < root->key)
	{
		root->left = delete_node(root->left, key);
	}
	else if (key > root->key)	// 찾는 키가 루트보다 크면
	{
		root->right = delete_node(root->right, key);
	}
	else
	{
		// 단말 노드 혹은 하나의 노드만을 가지고 있을 때
		if (root->left == NULL)
		{
			TreeNode* temp = root->right;
			free(root);
			return temp;
		}
		else if (root->right == NULL)
		{
			TreeNode* temp = root->left;
			free(root);
			return temp;
		}

		// 노드가 2개의 노드를 가지고 있을 때
		TreeNode* temp = min_value_node(root->right);

		root->key = temp->key;
		root->right = delete_node(root->right, temp->key);
	}

	return root;
}

void inorder(TreeNode* root)
{
	if (root)
	{
		inorder(root->left);
		printf("[%d] ", root->key);
		inorder((root->right));
	}
}

int main(void)
{
	TreeNode* root = NULL;

	root = insert_node(root, 30);
	root = insert_node(root, 20);
	root = insert_node(root, 10);
	root = insert_node(root, 40);
	root = insert_node(root, 50);
	root = insert_node(root, 60);

	printf("이진 탐색 트리 중위 순회 결과 \n");
	inorder(root);
	printf("\n\n");
	if (search(root, 30) != NULL)
		printf("이진 탐색 트리에서 30을 발견");
	else
		printf("이진 탐색 트리에서 30을 못 발견");
	return 0;
}*/