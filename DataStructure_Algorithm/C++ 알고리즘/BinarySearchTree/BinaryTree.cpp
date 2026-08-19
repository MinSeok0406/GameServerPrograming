#include "BinaryTree.h"
#include <iostream>
#include <stdlib.h>

bool insertData(BTree** root, int data)
{
    if (*root == nullptr)
    {
        BTree* newNode = (BTree*)malloc(sizeof(BTree));
        newNode->data = data;
        newNode->parent = nullptr;
        newNode->left = nullptr;
        newNode->right = nullptr;
        *root = newNode;
        return true;
    }

    BTree* node = *root;
    while (true)
    {
        if (node->data > data)
        {
            if (node->left == nullptr)
            {
                BTree* newNode = (BTree*)malloc(sizeof(BTree));
                newNode->data = data;
                newNode->parent = node;
                newNode->left = nullptr;
                newNode->right = nullptr;
                node->left = newNode;
                break;
            }
            node = node->left;
        }
        else if (node->data < data)
        {
            if (node->right == nullptr)
            {
                BTree* newNode = (BTree*)malloc(sizeof(BTree));
                newNode->data = data;
                newNode->parent = node;
                newNode->left = nullptr;
                newNode->right = nullptr;
                node->right = newNode;
                break;
            }
            node = node->right;
        }
        else if (node->data == data)
        {
            return false;
        }
    }

    return true;
}

bool deleteData(BTree** root, int data)
{
    BTree* node = searchNode(root, data);
    if (node == nullptr)
    {
        return false;
    }

    if (node->left != nullptr)
    {
        BTree* deletenode = searchMaxNode(node->left);
        node->data = deletenode->data;

        BTree* parent = deletenode->parent;
        BTree* child = deletenode->left;

        if (parent->left == deletenode)
        {
            parent->left = child;
        }
        else
        {
            parent->right = child;
        }

        if (child != nullptr)
        {
            child->parent = parent;
        }

        deleteNode(&deletenode);
    }
    else if (node->right != nullptr)
    {
        BTree* deletenode = searchMinNode(node->right);
        node->data = deletenode->data;

        BTree* parent = deletenode->parent;
        BTree* child = deletenode->right;

        if (parent->left == deletenode)
        {
            parent->left = child;
        }
        else
        {
            parent->right = child;
        }

        if (child != nullptr)
        {
            child->parent = parent;
        }

        deleteNode(&deletenode);
    }
    else
    {
        deleteNode(&node);
    }

    return true;
}

BTree* searchNode(BTree** root, int data)
{
    BTree* node = *root;

    while (true)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        if (node->data == data)
        {
            break;
        }
        else if (node->data > data)
        {
            node = node->left;
        }
        else if (node->data < data)
        {
            node = node->right;
        }
    }

    return node;
}

BTree* searchMinNode(BTree* root)
{
    BTree* node = root;
    while (true)
    {
        if (node->left == nullptr)
        {
            break;
        }

        node = node->left;
    }
    return node;
}

BTree* searchMaxNode(BTree* root)
{
    BTree* node = root;
    while (true)
    {
        if (node->right == nullptr)
        {
            break;
        }

        node = node->right;
    }
    return node;
}

bool traversalTree(BTree* root, int depth)
{
    if (root == nullptr)
    {
        return false;
    }

    if (depth == 0)
        printf("root: %d\n", root->data);

    if (root->left)
        printf("  [%d] %d -> %d (L)\n", depth, root->data, root->left->data);

    if (root->right)
        printf("  [%d] %d -> %d (R)\n", depth, root->data, root->right->data);

    traversalTree(root->left, depth + 1);
    traversalTree(root->right, depth + 1);

    return true;
}

bool deleteNode(BTree** root)
{
    BTree* node = *root;
    BTree* parent = node->parent;

    if (parent == nullptr)
    {
        *root = nullptr;
    }
    else if (parent->left == node)
    {
        parent->left = nullptr;
    }
    else
    {
        parent->right = nullptr;
    }

    free(node);
    return true;
}

bool Update(BTree** root)
{
    int num = 0;
    printf("1. InsertNode\n");
    printf("2. DeleteNode\n");
    printf("3. traversalTree\n");
    printf("4. exit\n");
    printf("Select Number: ");
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
        if (!Traversal(root))
        {
            printf("tour fail!!\n\n");
        }
        break;
    case 4:
        return false;
        break;
    }

    printf("\n\n");
    return true;
}

bool Insert(BTree** root)
{
    int data = 0;
    printf("insert data : ");
    scanf_s("%d", &data);

    if (!insertData(root, data))
    {
        return false;
    }

    return true;
}

bool Delete(BTree** root)
{
    int data = 0;
    printf("delete data : ");
    scanf_s("%d", &data);

    if (!deleteData(root, data))
    {
        return false;
    }

    return true;
}

bool Traversal(BTree** root)
{
    if (!traversalTree(*root, 0))
    {
        return false;
    }

    return true;
}