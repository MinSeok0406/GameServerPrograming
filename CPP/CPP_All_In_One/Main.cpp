#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <process.h>
#include <Windows.h>
#include <time.h>
#include <chrono>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")

constexpr uint64_t ITER = 50'000'000;

// 1) 같은 64B 라인: 고전적인 false sharing
struct SameLine
{
    alignas(64) uint64_t a;
    uint64_t b;
};

// 2) 다른 64B 라인 (a: 0번 라인, b: 1번 라인)
struct AdjLine
{
    alignas(128) uint64_t a;
    char pad[56];
    uint64_t b;
};

// 3) 아예 다른 128B 쌍
struct Far
{
    alignas(128) uint64_t a;
    char pad[120];
    uint64_t b;
};

struct Param
{
    uint64_t* p;
    int core;
};

long g_go = false;

unsigned __stdcall Worker(void* arg)
{
    Param* pr = (Param*)arg;
    SetThreadAffinityMask(GetCurrentThread(), 1ull << pr->core);
    while (!g_go) {}
    for (uint64_t i = 0; i < ITER; ++i)
        InterlockedIncrement64((volatile long long*)pr->p);
    return 0;
}

template <typename T>
void Test(const char* name)
{
    alignas(128) static T t;
    t.a = t.b = 0;
    g_go = 0;

    Param pa { &t.a, 0 };
    Param pb { &t.b, 2 };

    HANDLE h[2];
    h[0] = (HANDLE)_beginthreadex(nullptr, 0, Worker, &pa, 0, nullptr);
    h[1] = (HANDLE)_beginthreadex(nullptr, 0, Worker, &pb, 0, nullptr);

    auto start = chrono::steady_clock::now();
    g_go = 1;
    WaitForMultipleObjects(2, h, TRUE, INFINITE);
    chrono::duration<double, milli> dur = chrono::steady_clock::now() - start;

    printf("%-9s b-a offset=%3llu  %7.1f ms\n", name,
        (uint64_t)&t.b - (uint64_t)&t.a, dur.count());

    CloseHandle(h[0]);
    CloseHandle(h[1]);
}

int wmain()
{
    timeBeginPeriod(1);
    srand((unsigned int)time(nullptr));

    for (int round = 0; round < 3; ++round)
    {
        Test<SameLine>("SameLine");
        Test<AdjLine>("AdjLine");
        Test<Far>("Far");
        puts("");
    }

    return 0;
}