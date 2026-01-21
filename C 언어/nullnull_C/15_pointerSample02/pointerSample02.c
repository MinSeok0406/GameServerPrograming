#include <stdio.h>

int main(void)
{
	int x = 10;
	int* pnData = &x;

	printf("x : %d\n", x);
	*pnData = 20;
	printf("x : %d\n", x);

	return 0;
}
