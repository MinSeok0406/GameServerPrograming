#include <stdio.h>

void GetName(char* pszName, int nSize)
{
	printf("이름을 입력하세요. : ");
	gets_s(pszName, nSize);
}

int main(int argc, char* argv[])
{
	char szName[32] = { 0 };

	GetName(szName, sizeof(szName));
	printf("당신의 이름은 %s 입니다.\n", szName);
	return 0;
}
