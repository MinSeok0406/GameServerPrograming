#include <iostream>
#include <tchar.h>
#include <queue>
#include <stack>
#include <fcntl.h>
#include <io.h>
#include <conio.h>
#include <chrono>
#include <strsafe.h>
#include <WinSock2.h>
using namespace std;

#pragma comment(lib, "Winmm.lib")

extern "C" const IMAGE_DOS_HEADER __ImageBase;



namespace WidgetStuff
{
    class WidgetImpl
    {
    public:


    private:
        int a, b, c;
        std::vector<double> v;
    };

    template<typename T>
    class Widget
    {
    public:
        Widget() {}
        Widget(const Widget& rhs) {}

        Widget& operator=(const Widget& rhs)
        {
            *pImpl = *(rhs.pImpl);
            return *this;
        }

        void swap(Widget<T>& other)
        {
            using std::swap;

            swap(pImpl, other.pImpl);
        }

    private:
        WidgetImpl* pImpl;
    };

    template<typename T>
    void swap(Widget<T>& a, Widget<T>& b)
    {
        a.swap(b);
    }
}

int _tmain(int argc, TCHAR* argv[])
{
    timeBeginPeriod(1);

    using namespace WidgetStuff;

    Widget<int> w;
    Widget<int> w2;

    swap(w, w2);

    return 0;
}