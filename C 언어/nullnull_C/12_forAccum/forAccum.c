#include <stdio.h>

int main(void)
{
    int i = 1, nSum = 0;

    //while (i <= 10) {
    //    nSum += i;
    //    ++i;
    //}
    for (i = 1; i <= 10; ++i)
        nSum += i;
    printf("Total : %d", nSum);
	return 0;
}