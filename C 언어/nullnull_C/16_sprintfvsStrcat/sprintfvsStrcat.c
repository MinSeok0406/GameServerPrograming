#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
	char szPath[128] = { 0 };
	char szBuffer[128] = { 0 };

	printf("Input path: ");
	gets_s(szBuffer, sizeof(szBuffer));

	sprintf_s(szPath, _countof(szPath),
		"C:\\Program Files\\%s", szBuffer);
	puts(szPath);
	return 0;
}