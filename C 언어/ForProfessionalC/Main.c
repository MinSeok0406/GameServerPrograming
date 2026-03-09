#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

void InitCounter(int nData);
void IncreaseCounter();

extern int g_nCounter;

int main(int argc, char** argv)
{
    InitCounter(10);
    printf("%d\n", g_nCounter);
    IncreaseCounter();
    printf("%d\n", g_nCounter);

    return 0;
}