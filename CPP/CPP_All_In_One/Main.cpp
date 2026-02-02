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
using namespace std;

#pragma comment(lib, "ws2_32.lib")

//-------------------------------------------------------

class NewHandlerHolder
{
public:
    explicit NewHandlerHolder(new_handler nh) : handler(nh)
    {
        
    }

    ~NewHandlerHolder() { set_new_handler(handler); }

private:
    new_handler handler;

    NewHandlerHolder(const NewHandlerHolder&);
    NewHandlerHolder& operator=(const NewHandlerHolder&);
};

template<typename T>
class NewHandlerSupport
{
public:


    static std::new_handler set_new_handler(std::new_handler p) noexcept;
    static void* operator new(size_t size) noexcept;

private:
    static std::new_handler currentHandler;
};

template <typename T>
std::new_handler NewHandlerSupport<T>::set_new_handler(std::new_handler p) noexcept
{
    new_handler oldHandler = currentHandler;
    currentHandler = p;
    return oldHandler;
}

template <typename T>
void* NewHandlerSupport<T>::operator new(std::size_t size) noexcept
{
    try
    {
        NewHandlerHolder h(std::set_new_handler(currentHandler));
        return ::operator new(size);
    }
    catch (const bad_alloc& e)
    {
        // 에러 확인 후 프로그램 종료 혹은 무시하고 진행
    }
}

template<typename T>
std::new_handler NewHandlerSupport<T>::currentHandler = 0;

class Widget : public NewHandlerSupport<Widget>
{

};

void OutOfMem()
{
    cerr << "Memory Out" << "\n";
    std::abort();
}

int main()
{
    Widget::set_new_handler(OutOfMem);
    Widget* p1 = new Widget;

    string* ps = new string;

    Widget::set_new_handler(0);

    Widget* p2 = new Widget;

    cout << "문제없음!!" << "\n";

    return 0;
}
