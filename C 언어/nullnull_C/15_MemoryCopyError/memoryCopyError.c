#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char szBuffer[12] = { "HelloWorld" };
	char* pszData = NULL;

	pszData = (char*)malloc(sizeof(char) * 12);
	pszData = szBuffer;

	puts(pszData);
	return 0;
}
