#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#define CMD(NAME) \
    char NAME ## _cmd[256] = ""; \
    strcpy(NAME ## _cmd, #NAME);


int main(void)
{
    CMD(copy)
    CMD(paste)
    CMD(cut)

    char cmd[256];
    scanf_s("%s", cmd, sizeof(cmd));


    return 0;
}
