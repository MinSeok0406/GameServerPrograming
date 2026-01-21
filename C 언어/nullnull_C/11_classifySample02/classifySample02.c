#include <stdio.h>

int main(void)
{
    int nInput = 0;
    char ch = 'A';
    printf("점수를 입력하세요. : ");
    scanf_s("%d", &nInput);

    if (nInput >= 90)
        ch = 'A';

    else if (nInput >= 80)
        ch = 'B';
    else if (nInput >= 70)
        ch = 'C';
    else if (nInput >= 60)
        ch = 'D';
    else
        ch = 'F';

    printf("점수 : %d, 학점: %c\n", nInput, ch);
	return 0;
}