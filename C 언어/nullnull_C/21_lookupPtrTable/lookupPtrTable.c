#include <stdio.h>

typedef enum ACTION { MOVE, JUMP, ATTACK, QUIT } ACTION;

int DoMove(int nParam) {
	puts("DoMove");
	return 0;
}
int DoJump(int nParam) {
	puts("DoJump");
	return 0;
}
int DoAttack(int nParam) {
	puts("DoAttack");
	return 0;
}

ACTION GetInputFromClient()
{
	//TO-DO:
	return MOVE;
}

int main(void)
{
	int (*functionArray[3])(int) = {
		DoMove, DoJump, DoAttack
	};

	ACTION act = 0;
	while ((act = GetInputFromClient()) != QUIT)
	{
		functionArray[act](0);
	}

	return 0;
}
