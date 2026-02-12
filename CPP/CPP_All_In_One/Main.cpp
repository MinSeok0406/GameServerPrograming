#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <mutex>
#include <cmath>
#include <format>
#include <list>
#include <optional>
#include <map>
#include <set>
#include <iterator>
#include <WinSock2.h>
#include <new>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

const int theAnswer = 42;

auto x = theAnswer;
auto y = &theAnswer;

template<typename T>
class Widget;

template<typename T>
void f(const T& param)
{
    cout << typeid(T).name() << "\n";
    cout << typeid(param).name() << "\n";
}

int main()
{
    cout << typeid(x).name() << "\n";
    cout << typeid(y).name() << "\n";

    int a = 0;
    f(a);

    return 0;
}
