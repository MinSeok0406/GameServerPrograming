//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//
//// 문제 17-1-1
//int* maxPtr;
//int* minPtr;
//int arr[5] = {3, 1, 7, -2 ,5};
//
//void MaxAndMin(int** maxP, int** minP, int ary[])
//{
//    *maxP = &ary[0];
//    *minP = &ary[0];
//    for (int i = 1; i < 5; i++)
//    {
//        if (**maxP < ary[i])
//        {
//            *maxP = &ary[i];
//        }
//        
//        if (**minP > ary[i])
//        {
//            *minP = &ary[i];
//        }
//    }
//}
//
//int main(void)
//{
//    MaxAndMin(&maxPtr, &minPtr, arr);
//    printf("최대값: %d, 최소값: %d\n", *maxPtr, *minPtr);
//
//    return 0;
//}