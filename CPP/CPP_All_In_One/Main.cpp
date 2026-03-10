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

class WidgetID
{
    
};

unique_ptr<const Widget> loadWidget(WidgetID id);

shared_ptr<const Widget> fastLoadWidget(WidgetID id)
{
    static unordered_map<WidgetID, weak_ptr<const Widget>> cache;

    auto objPtr = cache[id].lock();

    if (!objPtr)
    {
        objPtr = loadWidget(id);
        cache[id] = objPtr;
    }

    return objPtr;
}


int main()
{
    auto spw = make_shared<Widget>();

    weak_ptr<Widget> wpw(spw);

    if (wpw.expired())
    {
        cout << "만료 O" << "\n";
    }
    else
    {
        cout << "만료 X" << "\n";
    }

    auto spw2 = wpw.lock();
    shared_ptr<Widget> spw3(wpw);

    spw = nullptr;

    //shared_ptr<Widget> spw4(wpw);

    if (wpw.expired())
    {
        cout << "만료 O" << "\n";
    }
    else
    {
        cout << "만료 X" << "\n";
    }
 
    return 0;
}
