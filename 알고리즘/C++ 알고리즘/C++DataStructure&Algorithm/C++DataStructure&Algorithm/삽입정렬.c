/*#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 10

int list[MAX_SIZE];
int n;

void insertion_sort(int list[], int n)
{
	int i, j, key;
	for (i = 1; i < n; i++)
	{
		key = list[i];
		for (j = i - 1; j >= 0 && list[j] > key; j--)
		{
			list[j + 1] = list[j];
		}
		list[j + 1] = key;
	}
}

int main(void)
{
	int i;
	n = MAX_SIZE;
	srand(time(NULL));

	for (i = 0; i < n; i++)
	{
		list[i] = rand() % 100;
	}

	insertion_sort(list, n);
	for (i = 0; i < n; i++)
		printf("%d ", list[i]);

	return 0;
}*/