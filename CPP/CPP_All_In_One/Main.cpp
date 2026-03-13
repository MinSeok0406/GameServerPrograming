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

class Widget
{
    
};

int computePriority()
{
    // TODO
    return 0;
}

void processWidget(shared_ptr<Widget> spw, int priority)
{
    
}

int main()
{
    auto uptr(make_unique<Widget>());
    unique_ptr<Widget> uptr2(new Widget);

    auto sptr(make_shared<Widget>());
    shared_ptr<Widget> sptr2(new Widget);

    processWidget(shared_ptr<Widget>(new Widget), computePriority());
    processWidget(make_shared<Widget>(), computePriority());

    auto sptr3 = make_shared<vector<int>>(10, 20);
    
    auto initList = {10, 20};
    auto sptr4 = make_shared<vector<int>>(initList);
 
    return 0;
}
