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

using UserInfo = tuple<string, string, size_t>;

template<typename E>
constexpr auto toUType(E enumerator) noexcept
{
    return static_cast<underlying_type_t<E>>(enumerator);
}

enum Color
{
    red = 1,
    yellow = 2,
};

//auto red = 1;

enum class Color2
{
    black = 1,
    blue = 2,
};

auto black = 2;

Color2 c = Color2::black;

enum Test : __int32;
enum class Status : long long;

int main()
{
    double d = Color::red * 1.5;
    //double d2 = Color2::black * 1.5;
    double d2 = static_cast<double>(Color2::black) * 1.5;

    Status s;
    Test t;

    cout << d << "\n";
    cout << d2 << "\n";

    enum class UserInfoFields
    {
        uiName,
        uiEmail,
        uiReputation
    };

    UserInfo uInfo;

    auto val = get<toUType(UserInfoFields::uiEmail)>(uInfo);

    return 0;
}
