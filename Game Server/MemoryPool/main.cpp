#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <memory>
#include <Windows.h>
#include "AllocInfo.h"
using namespace std;
using ll = long long;

class Widget
{
public:
	Widget() {}
	~Widget() {}

private:
	int _a;
	int _b;
	int _c;
	char _d;
	short _e;
};

int main()
{
	Widget* w = new Widget;

	delete[] w;

	return 0;
}