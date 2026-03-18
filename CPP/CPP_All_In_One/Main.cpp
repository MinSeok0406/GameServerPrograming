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
#include <chrono>
#include "Temp.h"
using namespace std;

#pragma comment(lib, "ws2_32.lib")

class Test
{
    
};

template<typename T>
void f(T&& param)
{
    
}

template<typename T>
void f(vector<T>&& param)
{

}

auto timeFuncInvocation = [](auto&& func, auto&&... params)
    {
        // TODO
        std::forward<decltype(func)>(func)(std::forward<decltype(params)>(params)(params));
        // TODO
    };

int main()
{
    Test t;

    f(t);           // 왼값 참조
    f(std::move(t));    // 오른값 참조

    vector<int> v;
    f(v);

    return 0;
}
