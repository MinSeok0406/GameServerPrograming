#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <process.h>
#include <Windows.h>
#include <time.h>
#include <chrono>
#include <vector>
#include <algorithm>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")

constexpr int ROUNDS = 10000000;

alignas(128) volatile BYTE g_val = 0;    // 관찰 대상 1바이트 (라인 X)
alignas(128) volatile LONG g_state = 0;  // 동기화용 (다른 라인)

std::vector<uint64_t> hit(ROUNDS), miss(ROUNDS);
int g_stale = 0;

unsigned __stdcall ThreadA(void*)
{
    SetThreadAffinityMask(GetCurrentThread(), 1 << 9);
    for (int i = 0; i < ROUNDS; ++i)
    {
        BYTE v = g_val;                              // 라인 X를 내 캐시로 가져옴
        uint64_t t1 = __rdtsc();
        v = g_val;                                   // 캐시 히트
        uint64_t t2 = __rdtsc();
        hit[i] = t2 - t1;

        g_state = 1;                                 // B에게 "읽었으니 써라"
        while (g_state != 2) {}                      // B가 쓸 때까지 대기

        BYTE before = v;
        t1 = __rdtsc();
        v = g_val;                                   // 무효화된 라인을 다시 읽음
        t2 = __rdtsc();
        miss[i] = t2 - t1;

        if (v == before) ++g_stale;                  // 낡은 값을 읽은 횟수
        g_state = 0;
    }
    return 0;
}

unsigned __stdcall ThreadB(void*)
{
    SetThreadAffinityMask(GetCurrentThread(), 1 << 11);
    for (int i = 0; i < ROUNDS; ++i)
    {
        while (g_state != 1) {}
        BYTE v = g_val;                              // 읽어서 S 상태로 공유
        g_val = v + 1;                               // 쓰기 → A의 라인 무효화
        g_state = 2;
    }
    return 0;
}

int wmain()
{
    timeBeginPeriod(1);
    srand((unsigned int)time(nullptr));

    HANDLE h[2];
    h[0] = (HANDLE)_beginthreadex(nullptr, 0, ThreadA, nullptr, 0, nullptr);
    h[1] = (HANDLE)_beginthreadex(nullptr, 0, ThreadB, nullptr, 0, nullptr);
    WaitForMultipleObjects(2, h, TRUE, INFINITE);

    std::sort(hit.begin(), hit.end());
    std::sort(miss.begin(), miss.end());
    printf("캐시 히트        (최대값): %llu cycles\n", hit[ROUNDS - 1]);
    printf("무효화 후 재읽기 (최대값): %llu cycles\n", miss[ROUNDS - 1]);
    printf("낡은 값을 읽은 횟수      : %d / %d\n", g_stale, ROUNDS);

    return 0;
}