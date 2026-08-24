#define _CRT_SECURE_NO_WARNINGS
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

void RBT_InsertNodeHelper(RBTNode** tree, RBTNode* newNode)
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
            RBT_InsertNodeHelper(&((*tree)->left), newNode);
        }
    }
}

RBTNode* RBT_RemoveNode(RBTNode** root, ElementType data)
{
    RBTNode* removed = nullptr;
    RBTNode* successor = nullptr;
    RBTNode* target = RBT_SearchNode(*root, data);

    if (target == nullptr)
    {
        return nullptr;
    }

    if (target->left == Nil || target->right == Nil)
    {
        removed = target;
    }
    else
    {
        removed = RBT_SearchMinNode(target->right);
        target->data = removed->data;
    }

    if (removed->left != Nil)
    {
        successor = removed->left;
    }
    else
    {
        successor = removed->right;
    }

    successor->parent = removed->parent;

    if (removed->parent == nullptr)
    {
        *root = successor;
    }
    else
    {
        if (removed == removed->parent->left)
        {
            removed->parent->left = successor;
        }
        else
        {
            removed->parent->right = successor;
        }
    }

    if (removed->Color == RBTNode::Color::BLACK)
    {
        RBT_RebuildAfterRemove(root, successor);
    }

    return removed;
}

void RBT_RebuildAfterInsert(RBTNode** root, RBTNode* x)
{
    // 4번 규칙을 위반하고 있는 동안에는 계속 반복
    while (x != *root && x->parent->Color == RBTNode::Color::RED)
    {
        // 부모 노드가 할아버지 노드의 왼쪽 자식일 경우
        if (x->parent == x->parent->parent->left)
        {
            RBTNode* uncle = x->parent->parent->right;
            // 삼촌이 빨간색 노드일 경우
            if (uncle->Color == RBTNode::Color::RED)
            {
                x->parent->Color = RBTNode::Color::BLACK;
                uncle->Color = RBTNode::Color::BLACK;
                x->parent->parent->Color = RBTNode::Color::RED;

                x = x->parent->parent;
            }
            else
            {
                // 삼촌이 검은색이고 x가 오른쪽 자식일 때
                if (x == x->parent->right)
                {
                    x = x->parent;
                    RBT_RotateLeft(root, x);
                }

                x->parent->Color = RBTNode::Color::BLACK;
                x->parent->parent->Color = RBTNode::Color::RED;

                RBT_RotateRight(root, x->parent->parent);
            }
        }
        else
        {
            RBTNode* uncle = x->parent->parent->left;
            if (uncle->Color == RBTNode::Color::RED)
            {
                x->parent->Color = RBTNode::Color::BLACK;
                uncle->Color = RBTNode::Color::BLACK;
                x->parent->parent->Color = RBTNode::Color::RED;

                x = x->parent->parent;
            }
            else
            {
                if (x == x->parent->left)
                {
                    x = x->parent;
                    RBT_RotateRight(root, x);
                }

                x->parent->Color = RBTNode::Color::BLACK;
                x->parent->parent->Color = RBTNode::Color::RED;

                RBT_RotateLeft(root, x->parent->parent);
            }
        }
    }

    // 루트 노드는 반드시 검은색이어야 함
    (*root)->Color = RBTNode::Color::BLACK;
}

void RBT_RebuildAfterRemove(RBTNode** root, RBTNode* successor)
{
    RBTNode* sibling = nullptr;

    while (successor->parent != nullptr && successor->Color == RBTNode::Color::BLACK)
    {
        if (successor == successor->parent->left)
        {
            sibling = successor->parent->right;

            if (sibling->Color == RBTNode::Color::RED)
            {
                sibling->Color = RBTNode::Color::BLACK;
                successor->parent->Color = RBTNode::Color::RED;
                RBT_RotateLeft(root, successor->parent);
            }
            else
            {
                if (sibling->left->Color == RBTNode::Color::BLACK && sibling->right->Color == RBTNode::Color::BLACK)
                {
                    sibling->Color = RBTNode::Color::RED;
                    successor = successor->parent;
                }
                else
                {
                    if (sibling->left->Color == RBTNode::Color::RED)
                    {
                        sibling->left->Color = RBTNode::Color::BLACK;
                        sibling->Color = RBTNode::Color::RED;

                        RBT_RotateRight(root, sibling);
                        sibling = successor->parent->right;
                    }

                    sibling->Color = successor->parent->Color;
                    successor->parent->Color = RBTNode::Color::BLACK;
                    sibling->right->Color = RBTNode::Color::BLACK;
                    RBT_RotateLeft(root, successor->parent);
                    successor = *root;
                }
            }
        }
        else
        {
            sibling = successor->parent->left;

            if (sibling->Color == RBTNode::Color::RED)
            {
                sibling->Color = RBTNode::Color::BLACK;
                successor->parent->Color = RBTNode::Color::RED;
                RBT_RotateRight(root, successor->parent);
            }
            else
            {
                if (sibling->right->Color == RBTNode::Color::BLACK && sibling->left->Color == RBTNode::Color::BLACK)
                {
                    sibling->Color = RBTNode::Color::RED;
                    successor = successor->parent;
                }
                else
                {
                    if (sibling->right->Color == RBTNode::Color::RED)
                    {
                        sibling->right->Color = RBTNode::Color::BLACK;
                        sibling->Color = RBTNode::Color::RED;

                        RBT_RotateLeft(root, sibling);
                        sibling = successor->parent->left;
                    }

                    sibling->Color = successor->parent->Color;
                    successor->parent->Color = RBTNode::Color::BLACK;
                    sibling->left->Color = RBTNode::Color::BLACK;
                    RBT_RotateRight(root, successor->parent);
                    successor = *root;
                }
            }
        }
    }

    successor->Color = RBTNode::Color::BLACK;
}

void RBT_PrintTree(RBTNode* node, int depth, int blackCount)
{
    int i = 0;
    char c = 'X';
    int v = -1;
    char cnt[100];

    if (node == nullptr || node == Nil)
    {
        return;
    }

    if (node->Color == RBTNode::Color::BLACK)
    {
        blackCount++;
    }

    if (node->parent != nullptr)
    {
        v = node->parent->data;

        if (node->parent->left == node)
        {
            c = 'L';
        }
        else
        {
            c = 'R';
        }
    }

    if (node->left == Nil && node->right == Nil)
    {
        sprintf(cnt, "--------- %d", blackCount);
    }
    else
    {
        sprintf(cnt, "");
    }

    for (i = 0; i < depth; ++i)
    {
        printf("  ");
    }

    printf("%d %s [%c, %d] %s\n", node->data, (node->Color == RBTNode::Color::RED) ? "RED" : "BLACK", c, v, cnt);

    RBT_PrintTree(node->left, depth + 1, blackCount);
    RBT_PrintTree(node->right, depth + 1, blackCount);
}

void RBT_RotateLeft(RBTNode** root, RBTNode* parent)
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
        *root = rightChild;
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
}

void RBT_RotateRight(RBTNode** root, RBTNode* parent)
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
        *root = leftChild;
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
}