#include <iostream>
#include <string.h>
#include <vector>
#include <Windows.h>
#include <process.h>
#include <chrono>
using namespace std;

#pragma comment (lib, "winmm.lib")

template<typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d, std::random_access_iterator_tag)
{
    iter += d;
}

template<typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d, std::bidirectional_iterator_tag)
{
    if (d >= 0)
    {
        while (d--)
        {
            ++iter;
        }
    }
    else
    {
        while (d++)
        {
            --iter;
        }
    }
}

template<typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d, std::input_iterator_tag)
{
    if (d >= 0)
    {
        while (d--)
        {
            ++iter;
        }
    }
}

template<typename IterT, typename DistT>
void advances(IterT& iter, DistT d)
{
    doAdvance(iter, d, typename std::iterator_traits<IterT>::iterator_category());
}

int wmain()
{
    timeBeginPeriod(1);

    vector<int> v;
    v.resize(1000000000);

    auto iter = v.begin();

    auto start = chrono::steady_clock::now();
    for (int i = 0; i < v.size(); ++i)
    {
        advances(iter, 1);
    }

    for (int i = 0; i < v.size(); ++i)
    {
        advances(iter, -1);
    }
    auto end = chrono::steady_clock::now();
    chrono::duration<double, std::milli> dur = end - start;
    cout << "Custom advance : " << dur.count() << "\n";

    start = chrono::steady_clock::now();
    for (int i = 0; i < v.size(); ++i)
    {
        advance(iter, 1);
    }

    for (int i = 0; i < v.size(); ++i)
    {
        advance(iter, -1);
    }
    end = chrono::steady_clock::now();
    chrono::duration<double, std::milli> dur2 = end - start;
    cout << "std::advance : " << dur2.count() << "\n";

    return 0;
}