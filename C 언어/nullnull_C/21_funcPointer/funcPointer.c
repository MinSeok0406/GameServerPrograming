#include <stdio.h>

int GetMax(int a, int b, int c)
{
	int nMax = a;
	if (b > nMax)	nMax = b;
	if (c > nMax)	nMax = c;
	return nMax;
}

int main(void)
{
	int(*pfGetMax)(int, int, int) = GetMax;
	printf("MAX: %d\n", pfGetMax(1, 3, 2));
	return 0;
}

