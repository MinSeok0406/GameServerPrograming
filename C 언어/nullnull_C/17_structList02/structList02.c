#include <stdio.h>

typedef struct USERDATA
{
	char szName[32];
	char szPhone[32];

	struct USERDATA* pNext;
} USERDATA;

int main(void)
{
	USERDATA userList[4] = {
		{ "±èµÎ½Ä", "1234", NULL },
		{ "ÀÌ¹ÌÇö", "2345", NULL },
		{ "±èºÀ¼®", "3456", NULL },
		{ "ÀåÈñ¼ö", "4567", NULL }
	};

	USERDATA * pUser = NULL;

	userList[0].pNext = &userList[1];
	userList[1].pNext = &userList[2];
	userList[2].pNext = &userList[3];
	userList[3].pNext = NULL;

	pUser = &userList[0];
	while (pUser != NULL)
	{
		printf("%s, %s\n", pUser->szName, pUser->szPhone);
		pUser = pUser->pNext;
	}
	return 0;
}
