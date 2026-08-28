#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <tchar.h>
#include <process.h>
#include <Windows.h>
#include <functional>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <stack>
#include <queue>
#include <list>
#include <time.h>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")

struct Node
{
    int a;
    int b;
    int c;
    short x;
    short y;
};

struct Comp
{
    bool operator()(const Node* lhs, const Node* rhs)
    {
        return lhs->a > rhs->a;
    }
};

priority_queue<Node*, vector<Node*>, Comp> pq;

int wmain(int argc, WCHAR* argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    timeBeginPeriod(1);
    srand((unsigned int)time(nullptr));

    for (auto i = 0; i < 10; ++i)
    {
        Node* node = new Node;
        node->a = rand() % 1000;
        node->b = rand() % 1000;
        node->c = rand() % 1000;
        node->x = rand() % 1000;
        node->y = rand() % 1000;
        pq.push(node);
    }

    return 0;
}
