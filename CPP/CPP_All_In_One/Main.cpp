#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <process.h>
#include <Windows.h>
#include <winerror.h>
#include <time.h>
#include <vector>
#include <array>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")

template<typename T>
class Unique_ptr
{
public:
    Unique_ptr() noexcept : _ptr(nullptr)
    {
        
    }

    ~Unique_ptr()
    {
        delete _ptr;
    }

    explicit Unique_ptr(T* p) noexcept : _ptr(p)
    {

    }

    Unique_ptr(Unique_ptr&& other) noexcept : _ptr(other.release())
    {

    }

    Unique_ptr& operator=(Unique_ptr&& other) noexcept
    {
        reset(other.release());
        return *this;
    }

    T& operator*() const { return *_ptr; }
    T* operator->() const noexcept { return _ptr; }
    T* get() const noexcept { return _ptr; }

    T* release() noexcept
    {
        T* p = _ptr;
        _ptr = nullptr;
        return p;
    }

    void reset(T* p = nullptr) noexcept
    {
        T* old = _ptr;
        _ptr = p;
        delete old;
    }

    explicit operator bool() const noexcept { return _ptr != nullptr; }

private:
    Unique_ptr(const Unique_ptr& other) = delete;
    Unique_ptr& operator=(const Unique_ptr& other) = delete;

    T* _ptr;
};

class Widget
{
public:
    Widget()
    {
        cout << "Widget" << "\n";
    }

    ~Widget()
    {
        cout << "~Widget" << "\n";
    }

    Widget(Widget&& other) noexcept = default;
    Widget& operator=(Widget&& other) noexcept = default;
    Widget(const Widget& other) = default;
    Widget& operator=(const Widget& other) = default;

private:

};

int wmain()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    timeBeginPeriod(1);
    srand((unsigned int)time(nullptr));

    unique_ptr<Widget> uptr(new Widget);
    unique_ptr<Widget> uptr2 = move(uptr);

    return 0;
}