#include <stdio.h>

#define STRING(a) #a
#define PASTER(a, b) a##b

int main(void)
{
	int nData = 10;

	printf("%d\n", PASTER(nDa, ta));
	printf("%d\n", nData);
	printf("%s\n", STRING(nData));
	return 0;
}

