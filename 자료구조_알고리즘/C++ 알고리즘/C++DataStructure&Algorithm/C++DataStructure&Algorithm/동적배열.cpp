/*#include <iostream>
#include <vector>
using namespace std;

#define SIZE 10

template<class T>
class Vector
{
public:
	Vector()
	{
		
	}
	
	~Vector()
	{
		if (_data)
			delete[] _data;
	}

	void Push_Back(const T& value)
	{
		if (_size == _capacity)
		{
			int newCapacity = static_cast<int>(_capacity * 1.5);
			if (newCapacity == _capacity)
				newCapacity++;

			Reserve(newCapacity);
		}

		_data[_size++] = value;
	}

	void Pop_Back()
	{
		if (_size == 0)
			return;

		T* newData = new T[_size - 1];

		for (int i = 0; i < _size - 1; i++)
		{
			newData[i] = _data[i];
		}

		if (_data)
			delete[] _data;

		_data = newData;
	}

	void Reserve(int n)
	{
		if (n <= _capacity)
			return;

		_capacity = n;

		T* newData = new T[_capacity];

		for (int i = 0; i < _size; i++)
		{
			newData[i] = _data[i];
		}

		if (_data)
			delete[] _data;

		_data = newData;
	}

	void Clear()
	{
		if (_data)
		{
			delete[] _data;
			_data = new T[_capacity];
		}

		_size = 0;
	}

	T& operator[](const int value) { return _data[value]; }

	int Size()
	{
		return _size;
	}

	int Capacity()
	{
		return _capacity;
	}


private:
	T*  _data = nullptr;
	int _size = 0;
	int _capacity = 0;
};

vector<string> solution(vector<string> strings, int n) {
	vector<string> answer;
	vector<pair<string, int>> v;

	for (int i = 0; i < strings.size(); i++)
	{
		v.push_back(make_pair(strings[n], i));
		cout << v[i].first << " " << v[i].second << endl;
	}

	return answer;
}


int main(void)
{
	Vector<int> v;

	for (int i = 0; i < 100; i++)
	{
		v.Push_Back(i);
		cout << v[i] << " " << v.Size() << " " << v.Capacity() << endl;
	}

	v.Clear();
	cout << v.Size() << " " << v.Capacity() << endl;

	return 0;
}*/