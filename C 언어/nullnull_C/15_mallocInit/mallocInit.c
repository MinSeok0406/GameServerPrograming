#include <stdio.h>
#include <stdlib.h>

int main(void) {
	char szBuffer[] = { "Hello" };
	char* pszBuffer = "Hello";
	char* pszData = NULL;

	pszData = (char*)malloc(sizeof(char) * 6);
	pszData[0] = 'H';
	pszData[1] = 'e';
	pszData[2] = 'l';
	pszData[3] = 'l';
	pszData[4] = 'o';
	pszData[5] = '\0';

	puts(szBuffer);
	puts(pszBuffer);
	puts(pszData);

	free(pszData);
	return 0;
}