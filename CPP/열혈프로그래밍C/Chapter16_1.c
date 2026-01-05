//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//
//// 巩力 16-1-1
////int arr[3][9];
//
//// 巩力 16-1-2
////int A[2][4] = {
////    {1, 2, 3, 4},
////    {5, 6, 7, 8}
////};
////
////int B[4][2];
//
//// 巩力 16-1-3
//int arr[5][5];
//
//int main(void)
//{
//    // 巩力 16-1-1
//    /*for (int i = 0; i < 3; i++)
//    {
//        for (int j = 0; j < 9; j++)
//        {
//            arr[i][j] = (i + 2) * (j + 1);
//        }
//    }
//
//    for (int i = 0; i < 3; i++)
//    {
//        for (int j = 0; j < 9; j++)
//        {
//            printf("%d ", arr[i][j]);
//        }
//        printf("\n");
//    }*/
//
//    // 巩力 16-1-2
//    /*for (int i = 0; i < 4; i++)
//    {
//        for (int j = 0; j < 2; j++)
//        {
//            B[i][j] = A[j][i];
//        }
//    }*/
//
//    // 巩力 16-1-3
//    for (int i = 0; i < 4; i++)
//    {
//        for (int j = 0; j < 4; j++)
//        {
//            scanf_s("%d", &arr[i][j]);
//        }
//    }
//
//    for (int i = 0; i < 4; i++)
//    {
//        int sum = 0;
//        int j;
//        for (j = 0; j < 4; j++)
//        {
//            sum += arr[i][j];
//        }
//        arr[i][j] = sum;
//    }
//
//    for (int i = 0; i < 5; i++)
//    {
//        int sum = 0;
//        int j;
//        for (j = 0; j < 4; j++)
//        {
//            sum += arr[j][i];
//        }
//        arr[j][i] = sum;
//    }
//
//    printf("\n");
//
//    for (int i = 0; i < 5; i++)
//    {
//        for (int j = 0; j < 5; j++)
//        {
//            printf("%d ", arr[i][j]);
//        }
//        printf("\n");
//    }
//
//    return 0;
//}