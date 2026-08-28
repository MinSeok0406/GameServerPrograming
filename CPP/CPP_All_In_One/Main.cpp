#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <tchar.h>
#include <process.h>
#include <Windows.h>
#include <functional>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <stack>
#include <queue>
#include <list>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")

class Top
{
};

class Middle : public Top
{

};

class Bottom : public Middle
{

};

template<typename T>
class SmartPointer
{
public:
    explicit SmartPointer(T* ptr)
    {

    }

    template<typename U>
    SmartPointer(const SmartPointer<U>& other) : _ptr(other.get())
    {

    }

    T* get() const { return _ptr; }

private:
    T* _ptr;
};

int wmain(int argc, WCHAR* argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    timeBeginPeriod(1);

    Top* t = new Middle;
    Top* t2 = new Bottom;

    SmartPointer<Top> shar = SmartPointer<Middle>(new Middle);
    //SmartPointer<Middle> s = SmartPointer<Top>(new Top);

    return 0;
}
