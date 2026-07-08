#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <tchar.h>
#include <process.h>
#include <Windows.h>
#include <algorithm>
#include <vector>
#include <string>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")

class Base
{
public:
    virtual ~Base() { }
    virtual void someMethod() { cout << "Base version" << "\n"; }
    
protected:
    int _protectedInt = 0;

private:
    int _privateInt = 0;
};

class Derived : public Base
{
public:
    void someMethod() override { cout << "Derived version" << "\n"; }
    void someOtherMethod() { }
};

int wmain(int argc, WCHAR* argv[])
{
    timeBeginPeriod(1);

    

    return 0;
}
