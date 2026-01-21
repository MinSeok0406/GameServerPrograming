#include <stdio.h>

int main(void)
{
	int x = 0, nResult = 0;

	++x;
	printf("%d\n", x);

	x++;
	printf("%d\n", x);

	nResult = ++x;
	printf("%d, %d\n", nResult, x);

	nResult = x--;
	printf("%d, %d\n", nResult, x);
	return 0;
}