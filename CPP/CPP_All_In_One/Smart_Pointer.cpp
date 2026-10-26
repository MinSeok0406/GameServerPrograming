/*#define _CRT_SECURE_NO_WARNINGS
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

template<typename T>
class Shared_ptr
{
public:
    Shared_ptr() : _ptr(nullptr), _refCount(nullptr) {}

    explicit Shared_ptr(T* ptr) : _ptr(ptr), _refCount(nullptr)
    {
        if (_ptr == nullptr)
            return;

        _refCount = new int(1);
    }

    ~Shared_ptr()
    {
        Release();
    }

    Shared_ptr(const Shared_ptr& other) noexcept
        : _ptr(other._ptr), _refCount(other._refCount)
    {
        if (_refCount != nullptr)
        {
            ++(*_refCount);
        }
    }

    Shared_ptr(Shared_ptr&& other) noexcept
        : _ptr(other._ptr), _refCount(other._refCount)
    {
        other._ptr = nullptr;
        other._refCount = nullptr;
    }

    void Swap(Shared_ptr& other) noexcept
    {
        std::swap(_ptr, other._ptr);
        std::swap(_refCount, other._refCount);
    }

    Shared_ptr& operator=(const Shared_ptr& other) noexcept
    {
        Shared_ptr(other).Swap(*this);
        return *this;
    }

    Shared_ptr& operator=(Shared_ptr&& other) noexcept
    {
        Shared_ptr(std::move(other)).Swap(*this);
        return *this;
    }

    T* Get() const noexcept { return _ptr; }

    T& operator*() const noexcept { return *_ptr; }
    T* operator->() const noexcept { return _ptr; }

    int32_t UseCount() const noexcept
    {
        return _refCount ? *_refCount : 0;
    }

    explicit operator bool() const noexcept { return _ptr != nullptr; }

    void Reset(T* ptr = nullptr)
    {
        Shared_ptr(ptr).Swap(*this);
    }

private:
    void Release() noexcept
    {
        if (_refCount != nullptr)
        {
            if (--(*_refCount) == 0)
            {
                delete _ptr;
                delete _refCount;
            }
        }

        _ptr = nullptr;
        _refCount = nullptr;
    }

    T* _ptr;
    int32_t* _refCount;
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

    Shared_ptr<Widget> sptr(new Widget);
    Shared_ptr<Widget> sptr2 = sptr;

    cout << "refCount : " << sptr.UseCount() << "\n";

    Shared_ptr<Widget> sptr3 = std::move(sptr2);

    cout << "refCount : " << sptr.UseCount() << "\n";

    return 0;
}*/