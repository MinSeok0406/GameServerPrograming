#include <iostream>
#include <time.h>
#include <process.h>
#include <Windows.h>
using namespace std;

#pragma comment(lib, "winmm.lib")

#define CLIENT 10

template<typename T>
class LockFreeStack
{
public:
    LockFreeStack() : _top(nullptr), _count(0)
    {

    }

    ~LockFreeStack() {}

    void push(T data);
    void pop(T& data);

private:
    struct Node
    {
        T data;
        Node* next;
    };

    long _count;
    Node* _top;
};

LockFreeStack<int> g_stack;

//------------------------------

HANDLE hEvent;
long cnt = 0;
uint32_t WINAPI threadProc(LPVOID param);

int wmain()
{
    timeBeginPeriod(1);

    hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    HANDLE hThread[CLIENT];

    for (auto i = 0; i < CLIENT; ++i)
    {
        hThread[i] = (HANDLE)_beginthreadex(NULL, 0, threadProc, NULL, 0, NULL);
    }

    Sleep(10);

    SetEvent(hEvent);

    WaitForMultipleObjects(CLIENT, hThread, TRUE, INFINITE);

    for (auto i = 0; i < CLIENT; ++i)
    {
        CloseHandle(hThread[i]);
    }

    return 0;
}

template<typename T>
void LockFreeStack<T>::push(T data)
{
    Node* newNode = new Node;
    newNode->data = data;

    Node* top;
    do
    {
        top = _top;
        newNode->next = top;

        if (InterlockedCompareExchangePointer((PVOID volatile*)&_top, newNode, top) == top)
        {
            InterlockedIncrement(&_count);
            break;
        }

    } while (true);
}

template<typename T>
void LockFreeStack<T>::pop(T& data)
{
    Node* top;

    do
    {
        top = _top;
        if (InterlockedCompareExchangePointer((PVOID volatile*)&_top, top->next, top) == top)
        {
            data = top->data;
            //delete top;
            InterlockedDecrement(&_count);
            break;
        }

    } while (true);
}

uint32_t __stdcall threadProc(LPVOID param)
{
    WaitForSingleObject(hEvent, INFINITE);

    int data = InterlockedAdd(&cnt, 1);

    while (true)
    {
        for (auto i = 0; i < CLIENT; ++i)
        {
            g_stack.push(data);
        }

        for (auto i = 0; i < CLIENT; ++i)
        {
            g_stack.pop(data);
        }
    }

    return 0;
}
