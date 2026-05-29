#include <iostream>
#include <string>
#include <Windows.h>
using namespace std;

#pragma comment(lib, "winmm.lib")

class AAA
{
public:
	AAA() {}
	~AAA() { }

	void someFunc() {};
};

class BBB : public AAA
{
public:
	BBB() {}
	~BBB() {}

	void someFunc2() {}
};

int main()
{
	timeBeginPeriod(1);

	
	
	

	return 0;
}

