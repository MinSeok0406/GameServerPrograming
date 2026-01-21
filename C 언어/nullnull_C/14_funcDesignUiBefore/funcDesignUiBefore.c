#include <stdio.h>

int GetFactorial(int nParam) {

	int nResult = 1, i = 0;
	if (nParam < 1 || nParam > 10) {
		puts("ERROR: 1~10 사이의 정수를 입력하세요.");
		return 0;
	}

	for (i = 1; i <= nParam; ++i)
		nResult *= i;
	return nResult;
}

int main(void) {

	printf("MAX: %d\n", GetFactorial(1));
	printf("MAX: %d\n", GetFactorial(5));

	printf("MAX: %d\n", GetFactorial(11));
	printf("MAX: %d\n", GetFactorial(10));
	return 0;
}

