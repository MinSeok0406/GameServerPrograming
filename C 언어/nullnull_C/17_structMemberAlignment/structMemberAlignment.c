#include <stdio.h>

#pragma pack(push, 1)
typedef struct USERDATA {
	char	ch;
	int		nAge;
	char	szName[5];
} USERDATA;
#pragma pack(pop)

int main(void)
{
	USERDATA user = { 'A', 10, "Hoon" };
	int data = 0x11223344;
	printf("%d\n", sizeof(USERDATA));
	return 0;
}

