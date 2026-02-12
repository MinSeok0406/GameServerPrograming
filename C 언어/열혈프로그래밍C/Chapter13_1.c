//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//
//
//
//int main(void)
//{
//    // 문제 13-1-1 / 13-1-2
//    /*int arr[5] = {1, 2, 3, 4, 5};
//    int* ptr = arr;
//
//    for (int i = 0; i < sizeof(arr) / sizeof(int); i++)
//    {
//        *ptr += 2;
//        ptr += 1;
//    }
//
//    for (int i = 0; i < sizeof(arr) / sizeof(int); i++)
//    {
//        printf("%d ", arr[i]);
//    }*/
//
//    // 문제 13-1-3
//    /*int arr[5] = {1, 2, 3, 4, 5};
//    int* ptr = &(arr[4]);
//
//    int sum = 0;
//    for (int i = 0; i < sizeof(arr) / sizeof(int); i++)
//    {
//        sum += *ptr;
//        ptr -= 1;
//    }
//
//    printf("%d\n", sum);*/
//
//    // 문제 13-1-4
//    /*int arr[6] = {1, 2, 3, 4, 5, 6};
//    int* ptr1 = arr;
//    int* ptr2 = arr + 5;
//
//    for (int i = 0; i < (sizeof(arr) / sizeof(int)) / 2; i++)
//    {
//        int temp = *ptr1;
//        *ptr1 = *ptr2;
//        *ptr2 = temp;
//
//        ptr1++;
//        ptr2--;
//    }
//
//    for (int i = 0; i < sizeof(arr) / sizeof(int); i++)
//    {
//        printf("%d ", arr[i]);
//    }*/
//
//    return 0;
//}