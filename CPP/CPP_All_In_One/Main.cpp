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

class Widget : public enable_shared_from_this<Widget>
{
public:
    template<typename... Ts>
    static std::shared_ptr<Widget> create(Ts&&... params);

    void process();

private:

};

vector<std::shared_ptr<Widget>> processedWidget;

void Widget::process()
{
    // TODO

    processedWidget.emplace_back(shared_from_this());
}



int main()
{
    auto delItem = [](Widget* pw)
        {
            // TODO
            delete pw;
        };

    auto delItem2 = [](Widget* pw)
        {
            // TODO
            delete pw;
        };

    std::unique_ptr<Widget, decltype(delItem)> upw(new Widget, delItem);
    std::unique_ptr<Widget, decltype(delItem2)> upw2(new Widget, delItem2);

    std::shared_ptr<Widget> spw(new Widget, delItem);
    std::shared_ptr<Widget> spw2(new Widget, delItem2);

    Widget* w = new Widget;
    auto sptr = make_shared<Widget>();
    auto ssptr = std::move(sptr);

    cout << sizeof(w) << "\n";
    cout << sizeof(sptr) << "\n";
    cout << sizeof(ssptr) << "\n";
    cout << sizeof(spw) << "\n";
    cout << sizeof(upw) << "\n";

    //vector<std::unique_ptr<Widget, decltype(delItem)>> vuptr {upw, upw2};

    vector<std::shared_ptr<Widget>> vsptr {spw, spw2};

    // 미정의 행동
    /*Widget* w2 = new Widget;
    std::shared_ptr<Widget> sptr2(w2);
    std::shared_ptr<Widget> ssptr2(w2);*/
 
    return 0;
}
