//#include <stdio.h>
//
//// 문제 11-1-1
////int arr[5];
////
////int MaxNum()
////{
////    int m = arr[0];
////    for (int i = 1; i < sizeof(arr) / sizeof(int); i++)
////    {
////        if (m < arr[i])
////        {
////            m = arr[i];
////        }
////    }
////
////    return m;
////}
////
////int MinNum()
////{
////    int m = arr[0];
////    for (int i = 1; i < sizeof(arr) / sizeof(int); i++)
////    {
////        if (m > arr[i])
////        {
////            m = arr[i];
////        }
////    }
////
////    return m;
////}
//
//// 문제 11-1-2
//char arr[] = "Good time";
//
//int main(void)
//{
//    // 문제 11-1-1
//    /*for (int i = 0; i < sizeof(arr) / sizeof(int); i++)
//    {
//        scanf_s("%d", arr + i);
//    }
//
//    int Max = MaxNum();
//    int Min = MinNum();
//    int sum = 0;
//    for (int i = 0; i < sizeof(arr) / sizeof(int); i++)
//    {
//        sum += arr[i];
//    }
//
//    printf("최대값: %d, 최소값: %d, 총합: %d\n", Max, Min, sum);*/
//
//    // 문제 11-1-2
//    for (int i = 0; i < sizeof(arr) / sizeof(char); i++)
//    {
//        printf("%c", arr[i]);
//    }
//
//
//
//    return 0;
//}