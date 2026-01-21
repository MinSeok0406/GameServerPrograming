#include <stdio.h>

int* TestFunc(void) {
	int nData = 10;
	return &nData;
}

int main(int argc, char* argv[]) {
	int* pnResult = NULL;
	pnResult = TestFunc();

	printf("%d\n", *pnResult);
	return 0;
}

