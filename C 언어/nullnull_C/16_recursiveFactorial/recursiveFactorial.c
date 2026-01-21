#include <stdio.h>

int GetFactorial(int nParam)
{
	int nResult = 0;
	if (nParam == 1) return 1;

	nResult = nParam * GetFactorial(nParam - 1);
	return nResult;
}

int main(void)
{
	printf("5! == %d\n", GetFactorial(5));
	return 0;
}