#include <stdio.h>
#include <string.h>

void main(void)
{
	char szBuffer[128] = { 0 };
	char szSet[128] = { 0 };
	char* pszStart = szBuffer;

	printf("Input string: ");
	gets_s(szBuffer, sizeof(szBuffer));

	printf("Input character set: ");
	gets_s(szSet, sizeof(szSet));

	while ((pszStart = strpbrk(pszStart, szSet)) != NULL)
	{
		printf("[%p] Index: %lld, %c\n",
			pszStart, pszStart - szBuffer, *pszStart);
		pszStart++;
	}
}
