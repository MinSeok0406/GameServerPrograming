#include <stdio.h>

int main(void)
{
    printf("%d\n", 1234);
    printf("%d, %d\n", 1234, -5678);
    printf("%+d, %+d\n", 1234, -5678);

    printf("%8d\n", 1234);
    printf("%08d\n", 1234);

    printf("%s\n", "Hello, World!");
	return 0;
}

