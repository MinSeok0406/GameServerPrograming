#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

int* Test()
{
    int nData = 10;
    return &nData;
}

int* Test2()
{
    int a = 5;
    return NULL;
}

int main(void)
{
    int* pData = NULL;
    pData = Test();
    Test2();

    printf("%d \n", *pData);

    return 0;
}