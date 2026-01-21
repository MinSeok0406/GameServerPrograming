#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	int* pList = NULL, * pNewList = NULL;
	int aList[3] = { 0 };

	pList = (int*)malloc(sizeof(int) * 3);
	memset(pList, 0, sizeof(int) * 3);

	pNewList = (int*)calloc(3, sizeof(int));

	for (int i = 0; i < 3; ++i)
		printf("pList[%d]의 값 : %d\n", i, pList[i]);

	for (int i = 0; i < 3; ++i)
		printf("pNewList[%d]의 값 : %d\n", i, pNewList[i]);

	free(pList);
	free(pNewList);
	return 0;
}