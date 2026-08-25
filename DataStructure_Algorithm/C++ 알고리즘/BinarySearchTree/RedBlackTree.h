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

RBTNode*    RBT_CreateNode(int newData);
bool        RBT_DropNode(RBTNode** root);
RBTNode*    RBT_SearchNode(RBTNode** tree, int target);
RBTNode*    RBT_SearchMinNode(RBTNode* tree);
RBTNode*    RBT_SearchMaxNode(RBTNode* tree);
bool        RBT_InsertNode(RBTNode** tree, RBTNode* newNode);
bool        RBT_InsertNodeHelper(RBTNode** tree, RBTNode* newNode);
bool        RBT_RebuildAfterInsert(RBTNode** root, RBTNode* x);
bool        RBT_DeleteNode(RBTNode** tree, int deleteData);
RBTNode*    RBT_DeleteNodeHelper(RBTNode** tree, RBTNode* node, RBTNode::COLOR& outColor);
bool        RBT_RebuildAfterDelete(RBTNode** root, RBTNode* x, RBTNode::COLOR color);
bool        RBT_RotateLeft(RBTNode** root, RBTNode* parent);
bool        RBT_RotateRight(RBTNode** root, RBTNode* parent);

bool        Update(RBTNode** root);
bool        Insert(RBTNode** root);
bool        Delete(RBTNode** root);
bool        Search(RBTNode** root);
bool        Render(RBTNode** root);
bool        BFS(RBTNode** root, int depth);