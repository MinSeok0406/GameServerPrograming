#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char szBuffer[32];
	int nResult = 0;

	printf("Input string: ");
	gets_s(szBuffer, sizeof(szBuffer));

	nResult = atoi(szBuffer);
	printf("%d\n", nResult);
	return 0;
}