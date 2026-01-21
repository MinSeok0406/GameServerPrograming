#include <stdio.h>

enum ACTION { MOVE, JUMP, ATTACK };
typedef enum COLOR { RED = 100, GREEN, BLUE } COLOR;

void DoAction(enum ACTION act)
{
	switch (act)
	{
	case MOVE:
		//TODO
		break;

	default:
		break;
	}
}

int main(void)
{
	enum ACTION act = MOVE;
	COLOR color = GREEN;

	printf("ACTION: %d, COLOR: %d\n", act, color);
	return 0;
}