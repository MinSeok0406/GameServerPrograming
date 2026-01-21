#include <stdio.h>

#define MAXLENGTH 32
#define CUTOFF	70

int main(void)
{
	char szName[MAXLENGTH] = { 0 };
	int nInput = 0;

	printf("점수를 입력하세요. : ");
	scanf_s("%d", &nInput);

	if (nInput >= CUTOFF)
		printf("합격입니다.\n");
	else
		printf("불합격입니다.\n");
	return 0;
}