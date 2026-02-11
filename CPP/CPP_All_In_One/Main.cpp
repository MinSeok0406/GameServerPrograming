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

template<typename Container, typename Index>
decltype(auto) authAndAccess(Container& c, Index i)
{
    return c[i];
}

class Widget
{
public:
};

decltype(auto) f1()
{
    int x = 0;
    return x;
}

decltype(auto) f2()
{
    int x = 0;
    return (x);
}

int main()
{
    vector<int> v(100, 0);

    authAndAccess(v, 10) = 5;

    cout << v[10] << "\n";

    Widget w;
    const Widget& cw = w;

    auto myWidget1 = cw;
    decltype(auto) myWidget2 = cw;

    return 0;
}
