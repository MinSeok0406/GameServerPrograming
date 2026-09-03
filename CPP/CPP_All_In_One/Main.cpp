#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <time.h>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")

template<typename T>
class NewHandlerSupport
{
public:
    static new_handler set_new_handler(new_handler p);
    static void* operator new(size_t size);

private:
    static new_handler currentHandler;
};

template<typename T>
new_handler NewHandlerSupport<T>::currentHandler = NULL;

class Widget : public NewHandlerSupport<Widget>
{

};

int wmain()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    timeBeginPeriod(1);
    srand((unsigned int)time(nullptr));

    

    return 0;
}

template<typename T>
new_handler NewHandlerSupport<T>::set_new_handler(new_handler p)
{
    new_handler oldHandler = currentHandler;
    currentHandler = p;
    return oldHandler;
}

template<typename T>
void* NewHandlerSupport<T>::operator new(size_t size)
{
    
    return ::operator new(size);
}
