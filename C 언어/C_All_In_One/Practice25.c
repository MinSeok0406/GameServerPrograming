/*#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct USERDATA
{
    char szName[32];
    char szPhone[32];

    struct USERDATA* pNext;
} USERDATA;

void Solve(USERDATA* user)
{
    if (user == NULL)
    {
        return;
    }

    printf("이름: %s 번호: %s \n", user->szName, user->szPhone);
    Solve(user->pNext);
    return;
}

int main(void)
{
    USERDATA userList[4] = {
        { "김두식", "1234", NULL},
        { "이미현", "2345", NULL},
        { "김봉석", "3456", NULL},
        { "장희수", "4567", NULL}
    };

    USERDATA* pUser = NULL;

    userList[0].pNext = &userList[1];
    userList[1].pNext = &userList[2];
    userList[2].pNext = &userList[3];
    userList[3].pNext = NULL;

    pUser = userList;

    Solve(pUser);

    return 0;
}*/