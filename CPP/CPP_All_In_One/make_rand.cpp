/*#include <iostream>
#include <WinSock2.h>
#include "Temp.h"
using namespace std;

#pragma comment(lib, "ws2_32.lib")

int num = 1;
int* p = &num;

void SRAND(unsigned int param)
{
    *p = param;
}

short RAND()
{
    int value = (*p) * 0x000343FD;
    value += 0x00269EC3;
    *p = value;

    value = (value >> 0x00000010);
    value &= 0x7fff;

    return value;
}

int main()
{
    SRAND(time(nullptr));
    short a = RAND();

    short b = rand();

    cout << a << "\n";

    return 0;
}*/