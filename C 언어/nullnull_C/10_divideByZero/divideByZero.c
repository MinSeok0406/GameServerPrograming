#include <stdio.h>

int main(void)
{
	int nInput = 0;
	printf("정수를 입력하세요.: ");

	scanf_s("%d", &nInput);
	printf("몫: %d\n", 10 / nInput);
	return 0;
}