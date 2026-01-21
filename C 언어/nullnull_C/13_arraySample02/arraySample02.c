#include <stdio.h>

int main(void)
{
	int aList[5] = {10, 20, 30, 40, 50};
	//int aList[] = {10, 20, 30, 40, 50};

	for (int i = 0; i < 5; ++i)
		printf("%d\t", aList[i]);
	putchar('\n');

	aList[0] = 100;
	aList[3] = 200;

	for (int i = 0; i < 5; ++i)
		printf("%d\t", aList[i]);
	putchar('\n');

	return 0;
}