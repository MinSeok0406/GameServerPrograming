#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <memory>
#include <Windows.h>
#include "AllocInfo.h"
using namespace std;
using ll = long long;

#pragma comment(lib, "winmm.lib")

class Widget
{
public:
	Widget() { cout << "Widget" << "\n"; }
	~Widget() { cout << "~Widget" << "\n"; }

private:
	int _a;
	int _b;
	int _c;
	char _d;
	short _e;
};

int main()
{
	timeBeginPeriod(1);

	Widget* w = new Widget[4];

	delete w;

	return 0;
}