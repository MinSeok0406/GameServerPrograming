#include <stdio.h>

int main(void)
{
	int aList[5] = { 0 };
	int* pnData = aList;
	//int *pnData = &aList[0];

	printf("aList[0] : %d\n", aList[0]);

	*pnData = 20;
	printf("aList[0] : %d\n", aList[0]);
	printf("pnData[0] : %d\n", pnData[0]);
	return 0;
}
