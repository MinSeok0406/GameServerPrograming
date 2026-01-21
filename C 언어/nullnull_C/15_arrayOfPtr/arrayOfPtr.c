#include <stdio.h>

int main(void)
{
	char* astrList[3] = { "Hello", "World", "String" };

	printf("%s\n", astrList[0]);
	printf("%s\n", astrList[1]);
	printf("%s\n", astrList[2]);

	printf("%s\n", astrList[0] + 1);
	printf("%s\n", astrList[1] + 2);
	printf("%s\n", astrList[2] + 3);

	printf("%c\n", astrList[0][3]);
	printf("%c\n", astrList[1][3]);
	printf("%c\n", astrList[2][3]);
	return 0;
}