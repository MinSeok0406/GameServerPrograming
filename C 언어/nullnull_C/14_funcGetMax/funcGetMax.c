#include <stdio.h>

int GetMax(int a, int b, int c)
{
	int nMax = a;
	if (b > nMax) nMax = b;
	if (c > nMax) nMax = c;

	return nMax;
}

int main(void) {

	int nResult = 0;

	printf("MAX: %d\n", GetMax(1, 2, 3));
	printf("MAX: %d\n", GetMax(2, 3, 1) * 2);
	printf("MAX: %d\n", nResult = GetMax(3, 1, 2));
	return 0;
}
