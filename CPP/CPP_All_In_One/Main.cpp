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

typedef vector<int>::iterator IterT;
typedef vector<int>::const_iterator ConstIterT;

vector<int> values;

vector<int> v;

template<typename C, typename V>
void findAndInsert(C& container, const V& targetVal, const V& insertVal)
{
    using std::cbegin;
    using std::cend;

    auto it = std::find(cbegin(container), cend(container), targetVal);
    container.insert(it, insertVal);
}

int main()
{
    ConstIterT ci = find(static_cast<ConstIterT>(values.begin()), static_cast<ConstIterT>(values.end()), 1983);

    //values.insert(static_cast<IterT>(ci), 1998);

    auto ci2 = find(v.cbegin(), v.cend(), 1983);
    v.insert(ci2, 1988);

    vector<int> v2;
    findAndInsert(v2, 10, 30);
    findAndInsert(v2, 20, 40);

    return 0;
}
