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
#include <map>
#include <set>
#include <iterator>
#include <WinSock2.h>
#include <new>
#include <functional>
#include <type_traits>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

enum class Type
{
    STOCK,
    BOND,
    REALESTATE,
};

class Investment
{
public:
    virtual ~Investment() {}
};

class Stock : public Investment
{
    
};

class Bond : public Investment
{

};

class RealEstate : public Investment
{
    
};

Type t;

template<typename... Ts>
auto makeInvestment(Ts&&... params)
{
    auto delInvmt = [](Investment* pInvestment)
        {
            // TODO
            delete pInvestment;
        };

    unique_ptr<Investment, decltype(delInvmt)> pInv(nullptr, delInvmt);

    if (t == Type::STOCK)
    {
        pInv.reset(new Stock(std::forward<Ts>(params)...));
    }
    else if (t == Type::BOND)
    {
        pInv.reset(new Bond(std::forward<Ts>(params)...));
    }
    else if (t == Type::REALESTATE)
    {
        pInv.reset(new RealEstate(std::forward<Ts>(params)...));
    }

    return pInv;
}

int main()
{
    
 
    return 0;
}
