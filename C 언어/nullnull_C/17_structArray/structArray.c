#include <stdio.h>

//구조체 선언 및 형 재선언
typedef struct USERDATA
{
	int nAge;
	char szName[32];
	char szPhone[32];
} USERDATA;

int main(void)
{
	//USERDATA 구조체 배열 선언 및 정의
	USERDATA userList[4] = {
		{ 10, "김두식", "1234" },
		{ 11, "이미현", "2345" },
		{ 17, "김봉석", "3456" },
		{ 12, "장희수", "4567" }
	};

	int i = 0;

	//배열 연산으로 각 USERDATA 인스턴스의 멤버 값을 출력한다.
	for (i = 0; i < 4; ++i)
		printf("%d살\t%s\t%s\n",

	userList[i].nAge, userList[i].szName,
		userList[i].szPhone);
	return 0;
}