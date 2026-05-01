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
    int arr[5] = { 0, };

    unsigned int ui = 0;
    int digit;
    int bytes;

    while (true)
    {
        ui = 0;
        printf("위치 (1~4) : ");
        scanf_s("%d", &digit);

        printf("값 [0~255] : ");
        scanf_s("%d", &bytes);

        arr[digit] = bytes;

        for (auto i = 1; i <= 4; i++)
        {
            printf("%d 번째 바이트 값 : %d\n", i, arr[i]);
        }
        printf("\n");

        for (auto i = 1; i <= 4; i++)
        {
            ui |= arr[i] << ((i - 1) * 8);
        }

        printf("전체 4바이트 값 : 0x%08x \n", ui);
        printf("\n");
    }

    return 0;
}*/