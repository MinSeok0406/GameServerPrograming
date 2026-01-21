#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int nData = 0;
	char szBuffer[128] = { 0 };
	FILE * fp = NULL;

	fopen_s(&fp, "fscanfTest.txt", "w");
	fprintf(fp, "%d,%s\n", 20, "Test");
	fclose(fp);

	fopen_s(&fp, "fscanfTest.txt", "r");
	fscanf_s(fp, "%d,%s",
		&nData, szBuffer, (unsigned)_countof(szBuffer));
	fclose(fp);

	printf("%d, %s\n", nData, szBuffer);
	return 0;
}