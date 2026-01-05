#include <iostream>
#include "BoundCheckArray.h"
using namespace std;

template <typename T>
BoundCheckArray<T>::BoundCheckArray(int len)
{
    account = new T[len];
}

template <typename T>
BoundCheckArray<T>::~BoundCheckArray()
{
    delete[] account;
}

template <typename T>
T& BoundCheckArray<T>::operator[](int idx)
{
    if (idx < 0 || idx >= arrlen)
    {
        cout << "Array index out of bound exception" << "\n";
        exit(1);
    }

    return account[idx];
}

template <typename T>
T BoundCheckArray<T>::operator[](int idx) const
{
    if (idx < 0 || idx >= arrlen)
    {
        cout << "Array index out of bound exception" << "\n";
        exit(1);
    }
    return account[idx];
}
