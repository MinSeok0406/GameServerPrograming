//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <string.h>
//
//// 도전 1
////int arr[10];
//
//// 도전 2
////int arr[1024];
//
//// 도전 3
////int arr[10];
//
//// 도전 4
////char arr[1000];
////
////void Palindrome(int len)
////{
////    int start = 0;
////    int end = len - 1;
////    for (int i = 0; i < len / 2; i++)
////    {
////        if (arr[start] != arr[end])
////        {
////            printf("회문이 아닙니다.\n");
////            return;
////        }
////
////        start++;
////        end--;
////    }
////
////    printf("회문 입니다.\n");
////    return;
////}
//
//// 도전 5
//int arr[7];
//
//void DesSort(int ary[], int len)
//{
//    for (int i = len - 1; i >= 0; i--)
//    {
//        for (int j = 0; j < i; j++)
//        {
//            if (ary[j] < ary[j + 1])
//            {
//                int temp = ary[j];
//                ary[j] = ary[j + 1];
//                ary[j + 1] = temp;
//            }
//        }
//    }
//}
//
//int main(void)
//{
//    // 도전 1
//    /*int num;
//    printf("총 10개의 숫자 입력\n");
//    for (int i = 0; i < sizeof(arr) / sizeof(int); i++)
//    {
//        printf("입력: ");
//        scanf_s("%d", &num);
//        arr[i] = num;
//    }
//    
//    int flag = 0;
//    printf("홀수 출력: ");
//    for (int i = 0; i < sizeof(arr) / sizeof(int); i++)
//    {
//        if (i == (sizeof(arr) / sizeof(int)) - 1)
//        {
//            if (arr[i] % 2 != 0)
//            {
//                printf("%d", arr[i]);
//            }
//
//            printf("\n");
//            continue;
//        }
//
//        if (arr[i] % 2 != 0)
//        {
//            printf("%d", arr[i]);
//            flag = 1;
//        }
//        else if (flag != 0)
//        {
//            printf(", ");
//        }
//    }
//
//    flag = 0;
//    printf("짝수 출력: ");
//    for (int i = 0; i < sizeof(arr) / sizeof(int); i++)
//    {
//        if (i == (sizeof(arr) / sizeof(int)) - 1)
//        {
//            if (arr[i] % 2 == 0)
//            {
//                printf("%d", arr[i]);
//                flag = 1;
//            }
//
//            printf("\n");
//            continue;
//        }
//
//        if (arr[i] % 2 == 0)
//        {
//            printf("%d", arr[i]);
//            flag = 1;
//        }
//        else if (flag != 0)
//        {
//            printf(", ");
//        }
//    }*/
//
//    // 도전 2
//    /*int num;
//    int p = 0;
//    scanf_s("%d", &num);
//    while (num > 0)
//    {
//        int temp = num % 2;
//        num /= 2;
//        arr[p++] = temp;
//    }
//
//    for (int i = p - 1; i >= 0; i--)
//    {
//        printf("%d", arr[i]);
//    }*/
//
//    // 도전 3
//    /*int num;
//    int len = sizeof(arr) / sizeof(int);
//    int start = 0;
//    int end = len - 1;
//    printf("총 10개의 숫자 입력\n");
//    for (int i = 0; i < len; i++)
//    {
//        printf("입력: ");
//        scanf_s("%d", &num);
//        if (num % 2 == 0)
//        {
//            arr[end--] = num;
//        }
//        else
//        {
//            arr[start++] = num;
//        }
//    }
//    printf("배열 요소의 출력 : ");
//    for (int i = 0; i < len; i++)
//    {
//        printf("%d ", arr[i]);
//    }*/
//
//    // 도전 4
//    /*printf("문자열 입력: ");
//    scanf_s("%s", arr, sizeof(arr));
//    int len = strlen(arr);
//    Palindrome(len);*/
//
//    // 도전 5
//    int len = sizeof(arr) / sizeof(int);
//    for (int i = 0; i < len; i++)
//    {
//        printf("입력: ");
//        scanf_s("%d", &arr[i]);
//    }
//    DesSort(arr, len);
//    for (int i = 0; i < len; i++)
//    {
//        printf("%d ", arr[i]);
//    }
//
//
//    return 0;
//}