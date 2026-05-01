/*#include "Temp.h"
#include <string>
#include <vector>
using namespace std;

int g_int = 3;

struct Widget::Impl
{
    string name;
    vector<double> data;
};

Widget::Widget() : pImpl(make_unique<Impl>())
{
}

Widget::~Widget() = default;

Widget::Widget(const Widget& rhs) : pImpl(nullptr)
{
    if (rhs.pImpl)
    {
        pImpl = make_unique<Impl>(*rhs.pImpl);
    }
}

Widget& Widget::operator=(const Widget& rhs)
{
    if (!rhs.pImpl)
    {
        pImpl.reset();
    }
    else if (!pImpl)
    {
        pImpl = make_unique<Impl>(*rhs.pImpl);
    }
    else
    {
        *pImpl = *rhs.pImpl;
    }

    return *this;
}

Widget::Widget(Widget&& rhs) = default;
Widget& Widget::operator=(Widget&& rhs) = default;*/