//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//
//// 문제 14-1-1
////int SquareByValue(int num)
////{
////    return num * num;
////}
////
////void SquareByReference(int* num)
////{
////    *num *= *num;
////}
//
//// 문제 14-1-2
//void Swap3(int* num1, int* num2, int* num3)
//{
//    int temp = *num1;
//    *num1 = *num3;
//    *num3 = *num2;
//    *num2 = temp;
//}
//
//int main(void)
//{
//    // 문제 14-1-1
//    /*int num;
//    scanf_s("%d", &num, sizeof(num));
//
//    int Square = SquareByValue(num);
//    SquareByReference(&num);
//
//    printf("%d %d\n", Square, num);*/
//
//    // 문제 14-1-2
//    int num1, num2, num3;
//    scanf_s("%d %d %d", &num1, &num2, &num3);
//    printf("변경 전 : %d %d %d\n", num1, num2, num3);
//    Swap3(&num1, &num2, &num3);
//    printf("변경 후 : %d %d %d\n", num1, num2, num3);
//
//    return 0;
//}