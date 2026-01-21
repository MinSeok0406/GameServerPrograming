#include <stdio.h>

int main(void)
{
	char szBuffer[32] = { "I am a boy." };

	const char* pszBuffer = szBuffer;
	szBuffer[0] = 'i';

	//*pszBuffer = 'i';
	return 0;
}

