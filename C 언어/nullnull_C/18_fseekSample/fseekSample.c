#include <stdio.h>

int main(void)
{
	FILE * fp = NULL;
	fopen_s(&fp, "Test.dat", "rb");

	fseek(fp, 0, SEEK_END);

	printf("size of Test.dat : %u\n", ftell(fp));
	fclose(fp);
	return 0;
}