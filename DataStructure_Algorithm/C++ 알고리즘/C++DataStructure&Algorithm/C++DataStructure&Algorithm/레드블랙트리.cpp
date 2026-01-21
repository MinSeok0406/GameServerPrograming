/*#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <deque>
#include <thread>
#include <chrono>
#include "BinarySearchTree.h"
using namespace std;

// Red-Black Tree
// 1) 모든 노드는 Red or Black
// 2) Root는 Black
// 3) Leaf(NIL)는 Black
// 4) Red 노드의 자식은 Black (연속해서 Red-Red X)
// 5) 각 노드로부터 ~ 리프까지 가는 경로들은 모두 같은 수의 Black

int main()
{
	BinarySearchTree bst;

	bst.Insert(300);
	bst.Print();
	this_thread::sleep_for(1s);

	bst.Insert(500);
	bst.Print();
	this_thread::sleep_for(1s);

	bst.Insert(100);
	bst.Print();
	this_thread::sleep_for(1s);

	bst.Insert(200);
	bst.Print();
	this_thread::sleep_for(1s);

	bst.Insert(600);
	bst.Print();
	this_thread::sleep_for(1s);

	bst.Insert(800);
	bst.Print();
	this_thread::sleep_for(1s);

	bst.Insert(900);
	bst.Print();
	this_thread::sleep_for(1s);

	bst.Insert(700);
	bst.Print();
	this_thread::sleep_for(1s);

	bst.Delete(900);
	bst.Print();
	this_thread::sleep_for(1s);

	bst.Delete(600);
	bst.Print();
	this_thread::sleep_for(1s);

	bst.Delete(200);
	bst.Print();
	this_thread::sleep_for(1s);

	return 0;
}*/