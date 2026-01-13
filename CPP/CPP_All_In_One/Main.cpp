#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <mutex>
using namespace std;

struct S
{

};

class Base
{
private:
    S s;
    int a = 4;
};

// S(1) + padding(3) + int(4) => Base (8)
// Base Å©±â(8) + char(1) + padding(7) + double(8) -> Derived (24)
class Derived : public Base
{
private:
    char c = 'a';
    double d = 8;
};

int main()
{
    cout << sizeof(S) << "\n";          // 1
    cout << sizeof(Base) << "\n";       // 8
    cout << sizeof(Derived) << "\n";    // 24

    return 0;
}