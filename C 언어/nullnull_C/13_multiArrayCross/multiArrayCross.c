#include <stdio.h>

int main(void) {

	int aList[5][5] = { 0 };
	int i = 0, j = 0, nCounter = 0;

	for (i = 0; i < 5; ++i) 
	{
		// Â¦¼ö Çà°ú È¦¼ö ÇàÀ» ±¸º°ÇÑ´Ù.
		if (i % 2 == 0)  // Â¦¼ö
			for (j = 0; j < 5; ++j)
				aList[i][j] = ++nCounter;
		else  // È¦¼ö
			for (j = 0; j < 5; ++j)
				aList[i][4 - j] = ++nCounter;
	}

	for (i = 0; i < 5; ++i)
	{
		for (j = 0; j < 5; ++j)
			printf("%d\t", aList[i][j]);
		putchar('\n');
	}

	return 0;
}