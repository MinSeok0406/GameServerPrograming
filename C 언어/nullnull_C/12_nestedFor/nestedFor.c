#include <stdio.h>

int main(void)
{
	int i = 0, j = 0;

	//여러 행을 출력하는 바깥쪽 반복문
	for (i = 0; i < 5; ++i)
	{
		//한 행에 *을 다섯 번 출력하는 안쪽 반복문
		for (j = 0; j < 5; ++j)
			printf("*\t");

		//한 행에 *을 모두 출력한 후 개행한다.
		putchar('\n');

	}

	return 0;
}