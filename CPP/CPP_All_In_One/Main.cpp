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
using namespace std;

#pragma comment(lib, "ws2_32.lib")

unordered_map<string, int> mp;

template<typename It>
void dwim(It b, It e)
{
    for (; b != e; ++b)
    {
        typename std::iterator_traits<It>::value_type currValue = *b;
        auto Value = *b;
    }
}

class Widget
{
    
};


int main()
{
    auto dare = [](const unique_ptr<Widget>& p1, const unique_lock<Widget>& p2) { return 0; };
    auto dare2 = [](const auto& p1, const auto& p2) { return 0; };

    std::function<bool(const unique_ptr<Widget>, const unique_lock<Widget>&)> func;

    return 0;
}
