#include <stdio.h>

int main(void)
{
	auto int aList[3] = { 10, 20, 30 };
	auto int i = 0;

	for (i = 0; i < 3; ++i)
		printf("%d\t", aList[i]);

	return 0;
}