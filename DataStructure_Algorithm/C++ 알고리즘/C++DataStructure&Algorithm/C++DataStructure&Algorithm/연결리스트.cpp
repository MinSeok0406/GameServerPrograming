/*#include <iostream>
#include <vector>
#include <list>
using namespace std;

template<class T>
class Node
{
public:
	Node() : _prev(nullptr), _next(nullptr), _data(T())
	{
		
	}

	Node(const T& value) : _prev(nullptr), _next(nullptr), _data(value)
	{
		
	}

public:
	Node*	_prev;
	Node*	_next;
	T		_data;
};

template<class T>
class Iterator
{
public:
	Iterator() : _node(nullptr)
	{
		
	}

	Iterator(Node<T>* node) : _node(node)
	{
		
	}

	// ++it
	Iterator& operator++()
	{
		_node = _node->_next;
		return *this;
	}

	// it++
	Iterator operator++(int)
	{
		Iterator<T> temp = *this;
		_node = _node->_next;
		return *this;
	}

	// --it
	Iterator& operator--()
	{
		_node = _node->_prev;
		return *this;
	}

	// it--
	Iterator operator--(int)
	{
		Iterator<T> temp = *this;
		_node = _node->_prev;
		return *this;
	}

	// *it
	T& operator*()
	{
		return _node->_data;
	}

	bool operator==(const Iterator& other)
	{
		return _node == other._node;
	}

	bool operator!=(const Iterator& other)
	{
		return _node != other._node;
	}

private:
	Node<T>* _node;
};

template<class T>
class List
{
public:
	List() : _size(0)
	{
		_head = new Node<T>();
		_tail = new Node<T>();
		_head->_next = _tail;
		_tail->_prev = _head;
	}

	~List()
	{
		while (_size > 0)
			Pop_Back();

		delete _head;
		delete _tail;
	}

public:
	// push_back
	void Push_Back(const T& value)
	{
		AddNode(_tail, value);

		/*_size++;
		Node<T>* newNode = new Node<T>(value);

		if (_size == 1)
		{
			_head->_next = newNode;
			_tail->_prev = newNode;
			newNode->_prev = _head;
			newNode->_next = _tail;
		}
		else
		{
			newNode->_prev = _tail->_prev;
			_tail->_prev->_next = newNode;
			newNode->_next = _tail;
			_tail->_prev = newNode;
		}#1#
	}

	// pop_back
	void Pop_Back()
	{
		RemoveNode(_tail->_prev);

		/*_size--;
		Node<T>* removeNode = new Node<T>();
		removeNode = _tail->_prev;

		_tail->_prev = _tail->_prev->_prev;
		_tail->_prev->_prev->_next = _tail;

		delete removeNode;#1#
	}

public:
	using iterator = Iterator<T>;

	iterator begin() { return iterator(_head->_next); }
	iterator end() { return iterator(_tail); }

	// insert
	iterator insert(iterator it, const T& value)
	{
		Node<T>* node = AddNode(it._node, value);
		return iterator(node);

		/*Node<T>* newNode = new Node<T>(value);

		newNode = it._node->_prev->_next;
		newNode->_prev = it._node->_prev;
		newNode->_next = it._node;
		it._node->_prev = newNode;

		++_size;

		return iterator(newNode);#1#
	}

	// erase
	iterator erase(iterator it)
	{
		Node<T>* node = RemoveNode(it._node);
		return iterator(node);
	}

private:
	Node<T>* AddNode(Node<T>* before, const T& value)
	{
		Node<T>* newNode = new Node<T>(value);

		before->_prev->_next = newNode;
		newNode->_prev = before->_prev;

		before->_prev = newNode;
		newNode->_next = before;

		_size++;

		return newNode;
	}

	Node<T>* RemoveNode(Node<T>* node)
	{
		Node<T>* prevNode = node->_prev;
		Node<T>* nextNode = node->_next;

		prevNode->_next = nextNode;
		nextNode->_prev = prevNode;

		delete node;

		_size--;

		return nextNode;
	}

	int size() { return _size; }

private:
	Node<T>*	_head;
	Node<T>*	_tail;
	int			_size;
};

int main()
{
	List<int> Li;

	return 0;
}*/