#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	char szBuffer[] = "Hello";
	char* pszBuffer = szBuffer;

	char* pszHeap = malloc(16);
	strcpy_s(pszHeap, 16, pszBuffer);
	//pszHeap = pszBuffer;

	puts(pszHeap);
	free(pszHeap);
	return 0;
}

