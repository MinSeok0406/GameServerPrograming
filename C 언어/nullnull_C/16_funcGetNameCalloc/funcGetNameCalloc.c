#include <stdio.h>
#include <stdlib.h>

char* GetName(void)
{
	char* pszName = NULL;

	pszName = (char*)calloc(32, sizeof(char));
	printf("이름을 입력하세요 : ");

	fgets(pszName, sizeof(char) * 32, stdin);
	return pszName;
}

int main(int argc, char* argv[])
{
	char* pszName = NULL;

	pszName = GetName();
	printf("당신의 이름은 %s입니다.\n", pszName);

	free(pszName);
	return 0;
}