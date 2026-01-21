#include <stdio.h>

int main(void)
{
	int aList[5] = { 0 };

	for (int i = 0; i < 5; ++i)
		scanf_s("%d", &aList[i]);

	for (int i = 0; i < 5; ++i)
		printf("%d\n", aList[i]);

	return 0;
}