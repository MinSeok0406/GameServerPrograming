#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum ACTION
{
    MOVE,
    JUMP,
    ATTACK,
    EXIT
} ACTION;

int DoMove(int i)
{
    puts("DoMove");
    return 0;
}

int DoJump(int i)
{
    puts("DoJump");
    return 0;
}

int DoAttack(int i)
{
    puts("DoAttack");
    return 0;
}

ACTION GetInputFromClient()
{
    return MOVE;
}

int main(void)
{
    int (*functionArray[3])(int) = {
    DoMove, DoJump, DoAttack
    };

    ACTION act = 0;
    while ((act = GetInputFromClient()) != EXIT)
    {
        functionArray[act](0);
    }

    return 0;
}