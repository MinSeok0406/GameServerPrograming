/*#include <iostream>
#include <tchar.h>
#include <queue>
#include <stack>
#include <fcntl.h>
#include <io.h>
#include <conio.h>
#include <chrono>
#include <WinSock2.h>
using namespace std;

#pragma comment(lib, "Winmm.lib")

#define SIZE 10

struct Node
{
    int key;
    int value;
    Node* node;
};

void AddData(Node** head)
{
    int key;
    int value;
    cout << "Key : ";
    cin >> key;

    cout << "Data : ";
    cin >> value;

    Node* newNode = new Node;
    newNode->key = key;
    newNode->value = value;
    newNode->node = nullptr;

    int index = key % SIZE;
    if (head[index] == nullptr)
    {
        head[index] = newNode;
    }
    else
    {
        Node* temp = head[index];
        while (temp->node != nullptr)
        {
            temp = temp->node;
        }

        temp->node = newNode;
    }

    cout << "\n\n";
    cout << "success insert Data" << "\n\n";
    cout << "press any key" << "\n";
    _getch();

    return;
}

void DeleteData(Node** head)
{
    int key;
    cout << "Key : ";
    cin >> key;

    int index = key % SIZE;
    if (head[index]->node == nullptr)
    {
        cout << "Key Not Found" << "\n\n";
    }
    else
    {
        Node* preNode = head[index];
        Node* temp = head[index]->node;
        while (temp != nullptr)
        {
            if (temp->key == key)
            {
                preNode->node = temp->node;
                delete temp;
                cout << "Delete Success" << "\n\n";
                break;
            }

            preNode = temp;
            temp = temp->node;
        }
    }

    cout << "press any key" << "\n";
    _getch();
}

void SeeData(Node** head)
{
    cout << "## HASH TABLE ##" << "\n\n";

    for (auto i = 0; i < SIZE; i++)
    {
        cout << "[0" << i << "]";
        Node* temp = head[i]->node;
        while (temp != nullptr)
        {
            cout << " > Key:" << temp->key << " | Data:" << temp->value;
            temp = temp->node;
        }
        cout << "\n";
    }

    cout << "\n";
    cout << "press any key" << "\n";
    _getch();
}

void FindData(Node** head)
{
    bool success = false;
    int key;
    cout << "Find Key : ";
    cin >> key;

    int index = key % SIZE;
    Node* temp = head[index]->node;
    while (temp != nullptr)
    {
        if (temp->key == key)
        {
            success = true;
            break;
        }

        temp = temp->node;
    }

    if (success)
    {
        cout << "Find Data : " << temp->value << "\n\n";
    }
    else
    {
        cout << "Not Found" << "\n\n";
    }

    cout << "press any key" << "\n";
    _getch();
}

int Print()
{
    int ret;
    cout << "## MENU ##" << "\n";
    cout << "1. Add Data" << "\n";
    cout << "2. Delete Data" << "\n";
    cout << "3. See Data" << "\n";
    cout << "4. Find Data" << "\n";
    cout << ":";
    cin >> ret;

    return ret;
}

int main()
{
    timeBeginPeriod(1);

    Node* adj[SIZE];

    for (auto i = 0; i < SIZE; i++)
    {
        adj[i] = new Node(-1, -1, nullptr);
    }

    while (true)
    {
        int ret = Print();
        switch (ret)
        {
        case 1:
            AddData(adj);
            system("cls");
            break;
        case 2:
            DeleteData(adj);
            system("cls");
            break;
        case 3:
            SeeData(adj);
            system("cls");
            break;
        case 4:
            FindData(adj);
            system("cls");
            break;
        default:
            cout << "\n\n";
            cout << "press any key" << "\n";
            _getch();
            system("cls");
            break;
        }
    }

    return 0;
}*/