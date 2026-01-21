#include <stdio.h>
#include <String.h>

int main(void)
{
	char szPath[128] = { "C:\\Program Files\\" };

	strcat_s(szPath, sizeof(szPath), "CHS\\");
	strcat_s(szPath, sizeof(szPath), "C programming");
	puts(szPath);

	strcpy_s(szPath, sizeof(szPath), "C:\\Program Files\\");

	//strcat_s(szPath + 17, sizeof(szPath) - 17, "CHS\\");
	//strcat_s(szPath + 17 + 4, sizeof(szPath) - 25, "C programming");
	strcpy_s(szPath + 17, sizeof(szPath) - 17, "CHS\\");
	strcpy_s(szPath + 17 + 4, sizeof(szPath) - 25, "C programming");

	puts(szPath);
	return 0;
}