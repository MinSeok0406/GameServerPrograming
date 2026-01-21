#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct USERDATA
{
	int nAge;
	char szName[32];
	char szPhone[32];
} USERDATA;

void GetUserData(USERDATA *pUser)
{
	USERDATA user = { 0 };
	scanf_s("%d%*c", &pUser->nAge);

	gets_s(pUser->szName, sizeof(pUser->szName));
	gets_s(pUser->szPhone, sizeof(pUser->szPhone));

	return;
}

int main(void)
{
	USERDATA user = { 0 };

	GetUserData(&user);
	printf("%d»ì\t%s\t%s\n",
		user.nAge, user.szName, user.szPhone);

	return 0;
}