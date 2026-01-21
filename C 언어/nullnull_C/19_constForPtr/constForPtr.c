#include <stdio.h>

int main(void)
{
	int nData = 10;
	const int* pnData = &nData;

	int* const pnNewData = &nData;

	//*pnData = 20;
	//pnNewData = NULL;
	return 0;
}

