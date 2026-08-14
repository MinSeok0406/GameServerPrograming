#include "RedBlackTree.h"
#include <iostream>
#include <stdlib.h>

extern RBTNode* Nil;

RBTNode* RBT_CreateNode(ElementType newData)
{
    RBTNode* newNode = new RBTNode;
    newNode->parent = nullptr;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->data = newData;
    newNode->Color = RBTNode::Color::BLACK;

    return newNode;
}

void RBT_DestoryNode(RBTNode* Node)
{
    free(Node);
}

void RBT_DestoryTree(RBTNode* tree)
{
    if (tree->right != Nil)
    {
        RBT_DestoryTree(tree->right);
    }
    
    if (tree->left != Nil)
    {
        RBT_DestoryTree(tree->left);
    }

    tree->left = Nil;
    tree->right = Nil;

    RBT_DestoryNode(tree);
}

RBTNode* RBT_SearchNode(RBTNode* tree, ElementType target)
{
    if (tree == Nil)
    {
        return nullptr;
    }

    if (tree->data > target)
    {
        return RBT_SearchNode(tree->left, target);
    }
    else if (tree->data < target)
    {
        return RBT_SearchNode(tree->right, target);
    }
    else
    {
        return tree;
    }
}

RBTNode* RBT_SearchMinNode(RBTNode* tree)
{
    if (tree == Nil)
    {
        return Nil;
    }

    if (tree->left == Nil)
    {
        return tree;
    }
    else
    {
        return RBT_SearchMinNode(tree->left);
    }
}

void RBT_InsertNode(RBTNode** tree, RBTNode* newNode)
{
    RBT_InsertNodeHelper(tree, newNode);

    newNode->Color = RBTNode::Color::RED;
    newNode->left = Nil;
    newNode->right = Nil;

    RBT_RebuildAfterInsert(tree, newNode);
}

void RBT_InsertNodeHelper(RBTNode** tree, RBTNode * newNode)
{
    if (*tree == nullptr)
    {
        *tree = newNode;
    }

    if ((*tree)->data < newNode->data)
    {
        if ((*tree)->right == Nil)
        {
            (*tree)->right = newNode;
            newNode->parent = (*tree);
        }
        else
        {
            RBT_InsertNodeHelper(&(*tree)->right, newNode);
        }
    }
    else if ((*tree)->data > newNode->data)
    {
        if ((*tree)->left == Nil)
        {
            (*tree)->left = newNode;
            newNode->parent = (*tree);
        }
        else
        {
            RBT_InsertNodeHelper(&(*tree)->left, newNode);
        }
    }
}

RBTNode* RBT_RemoveNode(RBTNode** root, ElementType target)
{
    return nullptr;
}

void RBT_RebuildAfterInsert(RBTNode** tree, RBTNode* newNode)
{}

void RBT_RebuildAfterRemove(RBTNode** root, RBTNode * x)
{}

void RBT_PrintTree(RBTNode * node, int depth, int blackCount)
{}

void RBT_RotateLeft(RBTNode** root, RBTNode * parent)
{}

void RBT_RotateRight(RBTNode** root, RBTNode * parent)
{}