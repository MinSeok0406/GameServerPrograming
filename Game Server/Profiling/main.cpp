#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <Windows.h>
using namespace std;

#pragma comment(lib, "winmm.lib")

class Widget
{
public:
	Widget() { cout << "Widget" << "\n"; }
	~Widget() { cout << "~Widget" << "\n"; }

private:

};

void Test() noexcept(false)
{
	throw 1;
}

void Test2()
{
	vector<Widget> v(10);

	Test();
}

int main()
{
	timeBeginPeriod(1);

	try
	{
		Test2();
	}
	catch (int e)
	{

	}

	return 0;
}

