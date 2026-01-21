#include <stdio.h>

int main(void)
{
	//실수 자료 형식의 메모리 크기(바이트 단위)를 출력
	printf("%d\n", sizeof(123.456F));
	printf("%d\n", sizeof(123.456));

	//float 형식을 실수 형식으로 출력
	printf("%f\n", 123.456F);	//float

	//double 형식을 실수 형식으로 출력
	printf("%f\n", 123.456);	//double
	printf("%lf\n", 123.456);	//long double
	printf("%g\n", 123.456);	//long double
	
	return 0;
}