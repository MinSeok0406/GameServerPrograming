#include <stdio.h>

int main(void)
{
	int nInput = 0;
	
	printf("점수를 입력하세요. : ");
	scanf_s("%d", &nInput);

	if (nInput >= 70)
		printf("합격입니다.\n");
	else
		printf("불합격입니다.\n");
	return 0;
}