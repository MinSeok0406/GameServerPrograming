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
#include <set>
using namespace std;

void separateOddsAndEvens(const int arr[], auto size, int** odds,
    auto* numOdds, int** evens, auto* numEvens)
{
    *numOdds = *numEvens = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (arr[i] % 2 == 1)
        {
            ++(*numOdds);
        }
        else
        {
            ++(*numEvens);
        }
    }

    *odds = new int[*numOdds];
    *evens = new int[*numEvens];

    size_t oddsPos = 0, evensPos = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (arr[i] % 2 == 1)
        {
            (*odds)[oddsPos++] = arr[i];
        }
        else
        {
            (*evens)[evensPos++] = arr[i];
        }
    }
}

void separateOddsAndEvens(const int arr[], auto size, int*& odds,
    auto& numOdds, int*& evens, auto& numEvens)
{
    numOdds = numEvens = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (arr[i] % 2 == 1)
        {
            ++numOdds;
        }
        else
        {
            ++numEvens;
        }
    }

    odds = new int[numOdds];
    evens = new int[numEvens];

    size_t oddsPos = 0, evensPos = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (arr[i] % 2 == 1)
        {
            odds[oddsPos++] = arr[i];
        }
        else
        {
            evens[evensPos++] = arr[i];
        }
    }
}

int main()
{
    int unSplit[] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int* oddNums { nullptr };
    int* evenNums { nullptr };
    size_t numOdds { 0 }, numEvens { 0 };

    separateOddsAndEvens(unSplit, sizeof(unSplit) / sizeof(int), &oddNums, &numOdds, &evenNums, &numEvens);


    return 0;
}