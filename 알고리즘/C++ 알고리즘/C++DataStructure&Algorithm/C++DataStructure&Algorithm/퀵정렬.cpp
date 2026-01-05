/*#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <deque>
using namespace std;

int Partition(vector<int>& v, int left, int right)
{
	int pivot = v[left];
	int low = left + 1;
	int high = right;

	// O(N)
	while (low <= high)
	{
		while (low <= right && pivot >= v[low])
		{
			low++;
		}

		while (high >= left + 1 && pivot <= v[high])
		{
			high--;
		}

		if (low < high)
		{
			swap(v[low], v[high]);
		}
	}

	swap(v[left], v[high]);

	return high;
}

// O(N^2) < ÃÖ¾Ç
// O(NlogN) < Æò±Õ
void QuickSort(vector<int>& v, int left, int right)
{
	if (left >= right)
	{
		return;
	}

	int pivot = Partition(v, left, right);
	QuickSort(v, left, pivot - 1);
	QuickSort(v, pivot + 1, right);
}

int main()
{
	vector<int> v = { 5, 2, 9, 1, 5, 6 };
	QuickSort(v, 0, v.size() - 1);
	for (int i = 0; i < v.size(); i++)
	{
		cout << v[i] << " ";
	}
	cout << endl;
	return 0;
}*/