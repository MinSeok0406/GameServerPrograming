/*#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <deque>
using namespace std;

#define SIZE 100

template<class T>
class Queue
{
public:
	Queue() : _front{ 0 }, _rear{ 0 }
	{
		
	}

	~Queue() = default;

public:
	// push
	void Push(const T& value)
	{
		arr[_rear] = value;
		_rear = (_rear + 1) % SIZE;
	}

	// pop
	void Pop()
	{
		if (Empty())
			return;

		arr[_front] = 0;
		_front = (_front + 1) % SIZE;
	}

	// back
	T& Back() { return arr[_rear - 1]; }

	// front
	T& Front() { return arr[_front]; }

	// empty
	bool Empty()
	{
		if (_front == _rear)
			return true;

		return false;
	}

	// size
	int Size() { return _rear - _front; }

private:
	T arr[SIZE];
	int _front;
	int _rear;
};

int main()
{
	Queue<int> q;

	for (int i = 0; i < 20; i++)
		q.Push(i);

	cout << q.Front() << " " << q.Back() << endl;

	while (q.Empty() == false)
	{
		int data = q.Front();
		q.Pop();
		cout << data << endl;
	}

	return 0;
}*/