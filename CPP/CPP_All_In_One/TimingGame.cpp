/*#include <iostream>
#include <tchar.h>
#include <queue>
#include <stack>
#include <fcntl.h>
#include <io.h>
#include <conio.h>
#include <chrono>
#include <WinSock2.h>
using namespace std;

#pragma comment(lib, "Winmm.lib")

int g_Timing[] = { 5, 10, 14, 17, 20, 25, 29, 31, 33 };
char g_Grade[9][100];

static int num = 0;
double second = ((double)CLOCKS_PER_SEC / 1000) / 4;

void Condition()
{
    auto currentTime = clock() / (double)CLOCKS_PER_SEC;
    auto Time = currentTime - g_Timing[num];

    if (Time > 1)
    {
        memcpy_s(g_Grade[num], sizeof(g_Grade[num]), "fail", sizeof("fail"));
        num++;
    }
}

void Content()
{
    if (_kbhit())
    {
        _getch();
        auto currentTime = clock() / (double)CLOCKS_PER_SEC;
        auto Time = abs(currentTime - g_Timing[num]);

        // 이 부분을 switch-case 문으로 교체해보자
        if (Time <= second)
        {
            memcpy_s(g_Grade[num], sizeof(g_Grade[num]), "Great", sizeof("Great"));
        }
        else if (Time <= second * 2)
        {
            memcpy_s(g_Grade[num], sizeof(g_Grade[num]), "Good", sizeof("Good"));
        }
        else if (Time <= second * 3)
        {
            memcpy_s(g_Grade[num], sizeof(g_Grade[num]), "NoGood", sizeof("NoGood"));
        }
        else if (Time < second * 4)
        {
            memcpy_s(g_Grade[num], sizeof(g_Grade[num]), "Bad", sizeof("Bad"));
        }
        else
        {
            memcpy_s(g_Grade[num], sizeof(g_Grade[num]), "fail", sizeof("fail"));
        }

        num++;
    }
}

void PrintTime()
{
    printf("%0.3lf Sec\n\n", clock() / (double)CLOCKS_PER_SEC);

    for (auto i = 0; i < sizeof(g_Timing) / sizeof(int); i++)
    {
        printf("%d Sec : %s\n", g_Timing[i], g_Grade[i]);
    }

    system("cls");
}

int main()
{
    timeBeginPeriod(1);

    while (true)
    {
        if (num == 9)
        {
            break;
        }

        Condition();

        Content();

        PrintTime();
    }

    return 0;
}*/