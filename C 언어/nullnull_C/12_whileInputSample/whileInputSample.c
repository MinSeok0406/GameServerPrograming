#include <stdio.h>

int main(void)
{
	char ch = 0;

	while ((ch = getchar()) != 'x') {
		putchar(ch);
	}

	return 0;
}
