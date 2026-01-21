#include <stdio.h>

int main(void)
{
	printf("%c\n", 'A');
	printf("%c\n", 'A' + 1);
	printf("%c\n", 'C');

	printf("%d\n", 'A');
	printf("%d\n", 'A' + 1);
	printf("%d\n", 'C');

	printf("%c\n", 65);
	printf("%c\n", 65 + 1);
	printf("%c\n", 67);
	return 0;
}