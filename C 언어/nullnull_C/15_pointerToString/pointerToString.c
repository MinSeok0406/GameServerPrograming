#include <stdio.h>
#include <string.h>

int main(void)
{
	char szBuffer[16] = { "Hello" };
	char* pszData = szBuffer;
	int nLength = 0;

	while (*pszData != '\0')
	{
		pszData++;
		nLength++;
	}

	printf("Length : %d\n", nLength);
	printf("Length : %d\n", strlen(szBuffer));
	printf("Length : %d\n", strlen("World"));

	return 0;
}