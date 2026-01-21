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
#include <set>
using namespace std;

consteval int GetConst(int a)
{
    return a * 100;
}

constexpr int getValue()
{
    return 10;
}

template<typename T>
constexpr T Value()
{
    T a = 10;
    return a;
}

int main()
{
    int a { 6 };
    int b { GetConst(Value<int>()) };

    
    return 0;
}