#include <stdio.h>
#include <string.h>

int main(void)
{
	FILE * fp = NULL;
	char szBuffer[512] = { 0 };

	fopen_s(&fp, "Test.txt", "w");
	fputs("Test\n", fp);
	fputs("String\n", fp);
	fputs("Data\n", fp);
	fclose(fp);

	fopen_s(&fp, "Test.txt", "r");
	if (fp == NULL)
		return;

	while (fgets(szBuffer, sizeof(szBuffer), fp))
	{
		printf("%s", szBuffer);
		memset(szBuffer, 0, sizeof(szBuffer));
	}

	fclose(fp);
	return 0;
}