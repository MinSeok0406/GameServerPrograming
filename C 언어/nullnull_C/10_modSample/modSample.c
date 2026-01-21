#include <stdio.h>

int main(void)
{
    int x = 0;
    scanf_s("%d", &x);

    printf("¸ò : %d\n", x / 3);
    printf("³ª¸ÓÁö : %d\n", x % 3);

	return 0;
}
