#pragma once

using ElementType = int;

struct RBTNode
{
    enum class Color
    {
        RED,
        BLACK
    } Color;

    RBTNode* parent;
    RBTNode* left;
    RBTNode* right;

    ElementType data;
};

RBTNode* RBT_CreateNode(ElementType newData);
void RBT_DestoryNode(RBTNode* Node);
void RBT_DestoryTree(RBTNode* tree);

RBTNode* RBT_SearchNode(RBTNode* tree, ElementType target);
RBTNode* RBT_SearchMinNode(RBTNode* tree);
void RBT_InsertNode(RBTNode** tree, RBTNode* newNode);
void RBT_InsertNodeHelper(RBTNode** tree, RBTNode* newNode);
RBTNode* RBT_RemoveNode(RBTNode** root, ElementType data);
void RBT_RebuildAfterInsert(RBTNode** root, RBTNode* x);
void RBT_RebuildAfterRemove(RBTNode** root, RBTNode* successor);

void RBT_PrintTree(RBTNode* node, int depth, int blackCount);
void RBT_RotateLeft(RBTNode** root, RBTNode* parent);
void RBT_RotateRight(RBTNode** root, RBTNode* parent);