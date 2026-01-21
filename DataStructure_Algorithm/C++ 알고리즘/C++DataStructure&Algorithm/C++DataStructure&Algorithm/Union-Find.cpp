/*#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <deque>
#include <windows.h>
using namespace std;

// 시간 복잡도 : 트리의 높이에 비례한 시간이 걸림
// 트리 구조를 이용한 상호 배타적 집합의 표현
struct Node
{
	Node* leader;
};

class NaiveDisjointSet
{
public:
	NaiveDisjointSet(int n) : _parent(n)
	{
		for (int i = 0; i < n; i++)
			_parent[i] = i;
	}

	// 루트 찾기
	int Find(int u)
	{
		if (u == _parent[u])
			return u;

		return Find(_parent[u]);
	}

	// u와 v를 합치기 (u가 v 밑으로
	void Merge(int u, int v)
	{
		u = Find(u);
		v = Find(v);

		if (u == v)
			return;

		_parent[u] = v;
	}

private:
	vector<int> _parent;
};

// 트리가 한쪽으로 기우는 문제를 해결
// 트리를 합칠 때, 항상 [높이가 낮은 트리를] [높이가 높은 트리] 밑으로 합침
// (Union-By-Rank)

// 시간 복잡도 = O(1)
class DisjointSet
{
public:
	DisjointSet(int n) : _parent(n), _rank(n, 1)
	{
		for (int i = 0; i < n; i++)
			_parent[i] = i;
	}

	// 루트 찾기
	int Find(int u)
	{
		if (u == _parent[u])
			return u;

		return _parent[u] = Find(_parent[u]);
	}

	// u와 v를 합치기 (u가 v 밑으로)
	void Merge(int u, int v)
	{
		u = Find(u);
		v = Find(v);

		if (u == v)
			return;

		if (_rank[u] > _rank[v])
			swap(u, v);

		// rank[u] <= rank[v] 보장
		_parent[u] = v;

		if (_rank[u] == _rank[v])
			_rank[v]++;
	}

private:
	vector<int> _parent;
	vector<int> _rank;
};

int main()
{
	DisjointSet p(8);
	int a = p.Find(4);
	int b = p.Find(2);
	int c = p.Find(5);

	p.Merge(1, 7);
	p.Merge(7, 6);
	p.Merge(3, 7);

	return 0;
}*/