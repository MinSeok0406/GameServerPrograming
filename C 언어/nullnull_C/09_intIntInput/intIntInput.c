#include <stdio.h>

int main(void)
{
    int x = 0, y = 0;

    printf("두 정수를 입력하세요 : ");
    // %d와 %d 사이에 공백이 없다는 점에 주의
    scanf_s("%d%d", &x, &y);

    printf("두 수의 합은 %d 입니다. \n", x + y);

    //char ch = 0;
    //printf("정수와 문자를 입력해 주세요 : ");
    //scanf_s("%d%c", &x, &ch, 1);
    //printf("입력한 수는 %d이고, 문자는 %c 입니다. \n", x, ch);
	return 0;
}