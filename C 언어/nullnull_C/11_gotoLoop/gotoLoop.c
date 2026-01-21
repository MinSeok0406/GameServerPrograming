#include <stdio.h>

int main(void)
{
	int nInput;

INPUT :
	printf("Input number : ");
	scanf_s("%d", &nInput);

	if (nInput < 0 || nInput > 10)
		goto INPUT;

	puts("End");
	return 0;
}