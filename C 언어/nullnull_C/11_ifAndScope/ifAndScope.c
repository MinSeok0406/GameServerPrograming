#include <stdio.h>

int main(void)
{
	int nAge = 0;

	printf("나이를 입력하세요. : ");
	scanf_s("%d", &nAge);

	if (nAge >= 20)
	{
		printf("처리 전, 당신의 나이는 %d세 입니다.\n", nAge);
		nAge = 20;
	}

	printf("당신의 나이는 %d세 입니다.\n", nAge);
	return 0;
}