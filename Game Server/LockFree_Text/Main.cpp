#include <iostream>
#include <time.h>
#include <process.h>
#include <Windows.h>
#include "CrashDump.h"
using namespace std;

#pragma comment(lib, "winmm.lib")

#define CLIENT 10

CrashDump g_crashDump;

enum class LogType
{
    PUSH = 1,
    POP = 2
};

struct StackLog
{
    uint32_t threadId;
    LogType type;
    void* oldTop;
    void* newTop;
};

constexpr uint64_t ce_logSize = 1 << 4;
StackLog g_log[ce_logSize];
int64_t g_logIndex = -1;
long g_stopLog = 0;

void writeLog(LogType type, void* oldTop, void* newTop);

template<typename T>
class LockFreeStack
{
public:
    LockFreeStack() : _top(nullptr)
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
        long alive;
    };

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
    srand((uint32_t)time(nullptr));

    for (auto i = 0; i < CLIENT * 10000; ++i)
    {
        g_stack.push(rand() % 100);
    }

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
    newNode->alive = 1;

    Node* top;
    do
    {
        top = _top;
        newNode->next = top;

        if (InterlockedCompareExchangePointer((PVOID volatile*)&_top, newNode, top) == top)
        {
            writeLog(LogType::PUSH, top, newNode);
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
        if (top == nullptr)
        {
            g_crashDump.Crash();
        }

        Node* next = top->next;
        if (InterlockedCompareExchangePointer((PVOID volatile*)&_top, next, top) == top)
        {
            writeLog(LogType::POP, top, next);

            if (top->next != next)
            {
                g_crashDump.Crash();
            }

            if (InterlockedExchange(&top->alive, 0) != 1)
            {
                g_crashDump.Crash();
            }

            data = top->data;
            delete top;
            break;
        }

    } while (true);
}

void writeLog(LogType type, void* oldTop, void* newTop)
{
    if (g_stopLog)
    {
        return;
    }

    int64_t cnt = InterlockedIncrement64(&g_logIndex);
    StackLog& log = g_log[cnt & (ce_logSize - 1)];
    log.threadId = GetCurrentThreadId();
    log.type = type;
    log.oldTop = oldTop;
    log.newTop = newTop;
}

uint32_t __stdcall threadProc(LPVOID param)
{
    WaitForSingleObject(hEvent, INFINITE);

    int data = InterlockedAdd(&cnt, 1);

    while (true)
    {
        for (auto i = 0; i < CLIENT * 1000; ++i)
        {
            g_stack.pop(data);
        }
    }

    return 0;
}

/*uint32_t __stdcall threadProc(LPVOID param)
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
}*/
