#include <stdio.h>

#define ADD(a, b) (a + b)

int Add(int a, int b)
{
	return a + b;
}

int main(void)
{
	printf("%d\n", Add(3, 4));
	printf("%d\n", ADD(3, 4));
	return 0;
}

