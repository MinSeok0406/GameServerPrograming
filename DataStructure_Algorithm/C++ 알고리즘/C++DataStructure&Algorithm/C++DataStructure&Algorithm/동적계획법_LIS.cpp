//#include <iostream>
//#include <vector>
//#include <list>
//#include <stack>
//#include <queue>
//#include <deque>
//#include <windows.h>
//using namespace std;
//
//int cache[100];
//vector<int> seq;
//
//int LIS(int pos)
//{
//	int& ret = cache[pos];
//	if (ret != -1)
//		return ret;
//
//	ret = 1;
//
//	// ±¸ÇÏ±â
//	for (auto next = pos + 1; next < seq.size(); next++)
//	{
//		if (seq[pos] < seq[next])
//		{
//			ret = max(ret, 1 + LIS(next));
//		}
//	}
//
//	return ret;
//}
//
//int main()
//{
//	::memset(cache, -1, sizeof(cache));
//
//	seq = vector<int>{ 1, 9, 2, 5, 7 };
//
//	int ret = 0;
//	for (auto pos = 0; pos < seq.size(); pos++)
//	{
//		ret = max(ret, LIS(pos));
//	}
//
//	cout << ret << endl;
//
//	return 0;
//}