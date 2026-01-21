#include <stdio.h>

void PrintString(const char* pszParam)
{
	puts(pszParam);
}

int main(void)
{
	char szBuffer[32] = { "I am a boy." };

	PrintString(szBuffer);
	PrintString("You are a girl.");
	return 0;
}

