#include <stdio.h>

#define _MSGTEST_

#ifdef _MSGTEST_
#define MYMESSAGE "I am a boy."
#else
#define MYMESSAGE "You are a girl."
#endif

int main(void)
{
	puts(MYMESSAGE);
	return 0;
}

