//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//
//// 巩力 11-2-1
////char arr[100];
//
//// 巩力 11-2-2
////char arr[100];
////char ret[100];
//
//// 巩力 11-2-3
//char arr[100];
//
//int main(void)
//{
//    // 巩力 11-2-1
//    /*int cnt = 0;
//    scanf_s("%s", arr, sizeof(arr));
//    while (arr[cnt] != '\0')
//    {
//        cnt++;
//    }
//    printf("%d\n", cnt);*/
//
//    // 巩力 11-2-2
//    /*int cnt = 0;
//    scanf_s("%s", arr, sizeof(arr));
//    while (arr[cnt] != '\0')
//    {
//        cnt++;
//    }
//    cnt--;
//
//    int i = 0;
//    while (cnt >= 0)
//    {
//        ret[i] = arr[cnt];
//        i++;
//        cnt--;
//    }
//    printf("%s\n", ret);*/
//
//    // 巩力 11-2-3
//    scanf_s("%s", arr, sizeof(arr));
//
//    int cnt = 0;
//    while (arr[cnt] != '\0')
//    {
//        cnt++;
//    }
//
//    int temp = (int)arr[0];
//    for (int i = 1; i < cnt; i++)
//    {
//        if (temp < (int)arr[i])
//        {
//            temp = (int)arr[i];
//        }
//    }
//
//    printf("%c", temp);
//
//
//    return 0;
//}