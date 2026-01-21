#include <stdio.h>

void PutData(char* pszParam)
{
	if (*pszParam == '\0')
		return;

	putchar(*pszParam);
	PutData(pszParam + 1);
}

int main(void)
{
	PutData("TestData");
	putchar('\n');
	return 0;
}

