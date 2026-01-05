#pragma once

template <typename T>
class BoundCheckArray
{
private:
    T* account;
    int arrlen;

    BoundCheckArray(const BoundCheckArray& ac) {}
    BoundCheckArray& operator=(const BoundCheckArray& ac) {}

public:
    BoundCheckArray(int len = 100);
    ~BoundCheckArray();

    T& operator[](int idx);
    T operator[](int idx) const;
    int GetArrLen() const { return arrlen; }
};



