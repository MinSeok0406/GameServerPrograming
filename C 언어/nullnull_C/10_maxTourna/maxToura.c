#include <stdio.h>

int main(void)
{
    int nMax = 0;
    int a, b, c;

    scanf_s("%d%d%d", &a, &b, &c);

    nMax = a > b ? a : b;
    nMax = nMax > c ? nMax : c;

    printf("MAX : %d\n", nMax);
	return 0;
}