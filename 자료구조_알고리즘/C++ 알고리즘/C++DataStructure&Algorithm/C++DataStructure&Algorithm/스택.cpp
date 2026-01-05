/*#include <iostream>
#include <vector>
#include <list>
#include <stack>
using namespace std;


template<class T>
class Stack
{
public:
	Stack() : _top(-1)
	{
		
	}

	~Stack() = default;

	// push
	void Push(const T& value)
	{
		arr[++_top] = value;
	}

	// pop
	void Pop()
	{
		arr[_top--] = 0;
	}

	// empty
	bool Empty()
	{
		if (_top == -1)
			return true;

		return false;
	}

	// top
	T& Top() { return arr[_top]; }

	// size
	int Size() { return _top + 1; }


private:
	T arr[100];
	int _top;
};


int main()
{
	Stack<int> s;

	s.Push(3);
	s.Push(4);
	s.Push(5);
	s.Push(6);

	while (s.Empty() == false)
	{
		int data = s.Top();
		s.Pop();
		cout << data << endl;
	}

	return 0;
}*/