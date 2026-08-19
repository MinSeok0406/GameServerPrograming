#pragma once

struct BTree
{
	int data;
	BTree* parent;
	BTree* left;
	BTree* right;
};

bool insertData(BTree** root, int data);
bool deleteData(BTree** root, int data);
bool traversalTree(BTree* root, int depth);
bool deleteNode(BTree** root);

BTree* searchNode(BTree** root, int data);
BTree* searchMinNode(BTree* root);
BTree* searchMaxNode(BTree* root);

bool Update(BTree** root);
bool Insert(BTree** root);
bool Delete(BTree** root);
bool Traversal(BTree** root);