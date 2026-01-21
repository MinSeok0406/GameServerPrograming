#include <stdio.h>
#include <string.h>

int main(void)
{
	char szBuffer[12] = { "TestString" };
	char* pszData = "TestString";

	if (strcmp(szBuffer, pszData) == 0)
	{
	}

	printf("%d\n", strcmp(szBuffer, pszData));
	printf("%d\n", strcmp("TestString", pszData));
	printf("%d\n", strcmp("Test", "TestString"));
	return 0;
}