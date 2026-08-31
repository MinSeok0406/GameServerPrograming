#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <stack>
#include <queue>
#include <list>
#include <time.h>
#include <chrono>
#include "PQ_STL.h"
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")

priority_queue<int> pq;
PriorityQueue<int> PQ;

int wmain(int argc, WCHAR* argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    timeBeginPeriod(1);
    srand((unsigned int)time(nullptr));

/*    unsigned int t = timeGetTime();

    while (true)
    {
        if (timeGetTime() - t < 5000)
        {
            int randNum = rand() % 10000;
            pq.push(randNum);
            PQ.Push(randNum);
        }
        else
        {
            while (true)
            {
                if (pq.empty())
                {
                    break;
                }

                if (pq.top() != PQ.Top())
                {
                    __debugbreak();
                }
                else
                {
                    pq.pop();
                    PQ.Pop();
                }
            }

            t = timeGetTime();
        }
    }*/

    auto start = chrono::steady_clock::now();

    for (auto i = 1; i < 10'000'000; ++i)
    {
        PQ.Push(i);
    }

    for (auto i = 1; i < 10'000'000; ++i)
    {
        PQ.Pop();
    }

    auto end = chrono::steady_clock::now();
    chrono::duration<double, std::milli> d = end - start;
    cout << "Custom PQ : " << d.count() << "ms" << "\n";

    auto start2 = chrono::steady_clock::now();

    for (auto i = 1; i < 10'000'000; ++i)
    {
        pq.push(i);
    }

    for (auto i = 1; i < 10'000'000; ++i)
    {
        pq.pop();
    }

    auto end2 = chrono::steady_clock::now();
    chrono::duration<double, std::milli> d2 = end2 - start2;
    cout << "STL PQ : " << d2.count() << "ms" << "\n";

    return 0;
}
