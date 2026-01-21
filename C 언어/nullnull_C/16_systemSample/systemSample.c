#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char szCommand[512] = { 0 };
	printf("Input command: ");
	gets_s(szCommand, sizeof(szCommand));

	system(szCommand);
	// system("notepad.exe");
	return 0;
}