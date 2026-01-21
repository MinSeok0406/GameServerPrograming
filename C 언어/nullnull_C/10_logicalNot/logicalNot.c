#include <stdio.h>

int main(void)
{
	int nInput = 0, bResult = 0;

	scanf_s("%d", &nInput);
	bResult = !nInput;

	printf("Result : %d (1:True, 0:False)\n", bResult);
	return 0;
}