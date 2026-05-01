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
    unsigned int temp;
    unsigned char uch;

    scanf_s("%d", &temp);
    uch = (char)temp;

    int cnt = sizeof(uch) * 8;
    while (cnt--)
    {
        if (uch & 1)
        {
            printf("1");
        }
        else
        {
            printf("0");
        }

        uch >>= 1;
    }

    return 0;
}*/