#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <tchar.h>
#include <process.h>
#include <Windows.h>
#include <functional>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <stack>
#include <queue>
#include <list>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")

class Widget
{
public:
    Widget() { cout << "Widget" << "\n"; }
    Widget(const Widget& w) { cout << "Widget const" << "\n"; }
    ~Widget() { cout << "~Widget" << "\n"; }
    Widget(Widget&& w) noexcept { cout << "Widget&&" << "\n"; }
};

Widget Test()
{
    Widget w;
    return w;
}

int wmain(int argc, WCHAR* argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    timeBeginPeriod(1);

    Test();

    return 0;
}
