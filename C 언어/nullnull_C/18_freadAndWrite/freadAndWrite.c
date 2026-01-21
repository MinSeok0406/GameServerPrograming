#include <stdio.h>
#include <string.h>

typedef struct USERDATA
{
	char szName[32];
	char szPhone[32];
} USERDATA;

int main(void)
{
	FILE * fp = NULL;
	USERDATA UserData = { "Ho-sung", "123-1234" };

	fopen_s(&fp, "Test.dat", "wb");
	if (fp == NULL)
		return;

	fwrite(&UserData, sizeof(USERDATA), 1, fp);
	fclose(fp);

	memset(&UserData, 0, sizeof(UserData));
	fp = NULL;
	fopen_s(&fp, "Test.dat", "rb");
	if (fp == NULL)
		return;

	fread(&UserData, sizeof(USERDATA), 1, fp);
	puts(UserData.szName);
	puts(UserData.szPhone);
	fclose(fp);
	return 0;
}

