#include <stdio.h>

int main(void)
{
    char* astrList[3] = { "Hello", "World", "String" };
    char** ppstrList = astrList;
    char*** pppstrList = &ppstrList;

    puts(ppstrList[0]);
    puts(ppstrList[1]);
    puts(ppstrList[2]);

    puts(*pppstrList[0]);
    puts(*(*(pppstrList + 0) + 1));
	return 0;
}