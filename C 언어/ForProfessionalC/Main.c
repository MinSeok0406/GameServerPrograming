#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include "StringAvg.h"


int main(int argc, char** argv)
{
    int array[5];

    for (int i = 0; i < 5; i++)
    {
        array[i] = i + 4;
    }

    double average = avg(array, 5, NORMAL);
    printf("The average: %f\n", average);

    average = avg(array, 5, SQUARED);
    printf("The squared average: %f\n", average);

    return 0;
}