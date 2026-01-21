#include <stdio.h>

int main(void)
{
	int nInput = 0;			//1¹ø
	scanf_s("%d", &nInput);

	if (nInput > 10)
	{
		int nInput = 20;	//2¹ø
		printf("%d\n", nInput);

		if (nInput < 20)
		{
			int nInput = 30;//3¹ø
			printf("%d\n", nInput);
		}
	}

	printf("%d\n", nInput);
	return 0;
}