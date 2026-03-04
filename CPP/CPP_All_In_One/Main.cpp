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
#include <functional>
#include <type_traits>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

class Polynomial
{
public:
    using RootsType = vector<double>;

    RootsType roots() const
    {
        lock_guard<mutex> g(m);

        if (!rootsAreValid)
        {
            // TODO
            rootsAreValid = true;
        }

        return rootVals;
    }

private:
    mutable mutex m;
    mutable bool rootsAreValid { false };
    mutable RootsType rootVals {};
};

class Point
{
public:
    double distanceFromOrigin() const noexcept
    {
        callCount.fetch_add(1);

        return hypot(x, y);
    }

private:
    mutable atomic<unsigned> callCount { 0 };
    double x, y;
};

class Widget
{
public:
    int magicValue() const
    {
        if (cacheValue)
        {
            return cachedValue;
        }

        auto val1 = GetValue1();
        auto val2 = GetValue2();
        cachedValue = val1 + val2;
        cacheValue = true;
        return cachedValue;
    }

    int GetValue1() const
    {
        return 1;
    }

    int GetValue2() const
    {
        return 1;
    }

private:
    mutable atomic<bool> cacheValue { false };
    mutable atomic<int> cachedValue;
};

int main()
{
    

    return 0;
}
