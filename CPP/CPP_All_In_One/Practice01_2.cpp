/*#include <iostream>
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
#include <chrono>
#include "Temp.h"
using namespace std;

#pragma comment(lib, "ws2_32.lib")


int main()
{
    unsigned short ush = 0;
    int temp, temp2;

    while (true)
    {
        cout << "비트워치 : ";
        cin >> temp;

        cout << "OFF/ON [0,1] : ";
        cin >> temp2;

        if (temp < 1 || temp > 16)
        {
            cout << "비트 범위를 초과하였습니다." << "\n";
            continue;
        }

        if (temp2 == 1)
        {
            ush |= (1 << temp);
        }
        else
        {
            ush &= ~(1 << temp);
        }

        for (auto i = 16; i >= 1; i--)
        {
            cout << i << " 번 Bit : ";

            if (ush & (1 << i))
            {
                cout << "ON" << "\n";
            }
            else
            {
                cout << "OFF" << "\n";
            }
        }
    }

    return 0;
}*/