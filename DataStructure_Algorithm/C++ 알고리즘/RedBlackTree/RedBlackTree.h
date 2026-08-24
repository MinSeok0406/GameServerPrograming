#pragma once

struct RBTNode
{
    enum class COLOR
    {
        RED,
        BLACK
    } Color;

    RBTNode* parent;
    RBTNode* left;
    RBTNode* right;

    int data;
};

RBTNode* RBT_CreateNode(int newData);
RBTNode* RBT_SearchNode(RBTNode* tree, int target);
RBTNode* RBT_SearchMinNode(RBTNode* tree);
RBTNode* RBT_SearchMaxNode(RBTNode* tree);
bool RBT_InsertNode(RBTNode** tree, RBTNode* newNode);
bool RBT_InsertNodeHelper(RBTNode** tree, RBTNode* newNode);
bool RBT_RebuildAfterInsert(RBTNode** root, RBTNode* x);
bool RBT_RotateLeft(RBTNode** root, RBTNode* parent);
bool RBT_RotateRight(RBTNode** root, RBTNode* parent);