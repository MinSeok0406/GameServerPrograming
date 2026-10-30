#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <process.h>
#include <Windows.h>
#include <chrono>
#include <string>
#include <set>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")

class Widget
{
public:
    Widget() { cout << "Widget" << "\n"; }
    ~Widget() { cout << "~Widget" << "\n"; }
    Widget(const Widget& other) { cout << "Widget(Widget)" << "\n"; }
    Widget(Widget&& other) noexcept { cout << "Widget(Widget&&)" << "\n"; }
    Widget& operator=(const Widget& other) { cout << "Widget=" << "\n"; }
    Widget& operator=(Widget&& other) noexcept { cout << "Widget&&=" << "\n"; }
};

multiset<string> names;

template<typename T>
void logAndAddImpl(T&& name, std::false_type)
{
    auto now = chrono::steady_clock::now();
    cout << "template type func" << "\n";
    names.emplace(std::forward<T>(name));
}

void logAndAddImpl(int idx, std::true_type)
{
    cout << "int type func" << "\n";
}

template<typename T>
void logAndAdd(T&& name)
{
    logAndAddImpl(std::forward<T>(name), std::is_integral<std::remove_reference_t<T>>());
}

int wmain()
{
    timeBeginPeriod(1);

    

    return 0;
}