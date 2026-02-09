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

template<typename T>
void f(T param)
{
    const char* ptr = "des";
    param = ptr;
    param = "HW";
}

template<typename T>
void ff(T& param)
{
    cout << sizeof(param) << "\n";
}

int main()
{
    const char* const cccs = "Hello World";
    f(cccs);
    f("Hello World");

    const char name[] = "Hello";
    cout << sizeof(name) << "\n";
    ff(name);

    return 0;
}
