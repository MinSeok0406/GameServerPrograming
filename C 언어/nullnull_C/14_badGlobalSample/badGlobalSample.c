#include <stdio.h>

//이 프로그램 전체 기능과 관련된 주요 자료구조를 전역변수로 선언 및 정의
int g_aList[5] = { 0 };

//사용자로부터 값을 입력받아 전역변수 배열을 초기화한다.
void InitList(void)
{
	int i = 0;
	for (i = 0; i < 5; ++i)
	{
		printf("정수를 입력하세요. : ");
		scanf_s("%d", &g_aList[i]);
	}
}

//전역변수 배열에 저장된 값을 정렬하는 기능
//다른 int[5] 배열을 정렬하지는 못한다!
void SortList(void)
{
	int i = 0, j = 0, nTmp = 0;
	for (i = 0; i < 4; ++i)
		for (j = i + 1; j < 5; ++j)
			if (g_aList[j] < g_aList[i])
			{
				nTmp = g_aList[j];
				g_aList[j] = g_aList[i];
				g_aList[i] = nTmp;
			}
}

//전역변수 배열에 담긴 값을 출력
void PrintList(void)
{
	int i = 0;
	for (i = 0; i < 5; ++i)
		printf("%d\t", g_aList[i]);
	putchar('\n');
}

//프로그램의 흐름
int main(void)
{
	InitList();
	SortList();
	PrintList();

	return 0;
}