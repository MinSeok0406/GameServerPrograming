#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <process.h>
#include <Windows.h>
#include <time.h>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")

class Widget
{
public:
    Widget() { cout << "Widget" << "\n"; }
    ~Widget() { cout << "~Widget" << "\n"; }

    void Test()
    {
        a = 5;
        b = 7;
        cout << "Test" << "\n";
    }

private:
    int a = 0;
    int b = 1;
};

int wmain()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    timeBeginPeriod(1);
    srand((unsigned int)time(nullptr));

    Widget* w = new Widget;
    w->Test();

    delete w;

    return 0;
}