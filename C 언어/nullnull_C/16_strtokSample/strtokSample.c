#include <stdio.h>
#include <string.h>

void main(void)
{
	char szBuffer[128] = { "nData = x + y;\nnResult = a * b" };
	char* pszSep = " *+=;\n";
	char* pszToken = NULL;
	char* pszNextToken = NULL;

	pszToken = strtok_s(szBuffer, pszSep, &pszNextToken);
	while (pszToken != NULL)
	{
		puts(pszToken);
		pszToken = strtok_s(NULL, pszSep, &pszNextToken);
	}

	printf("\nszBuffer: %s\n", szBuffer);
}
