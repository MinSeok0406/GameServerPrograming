//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//// 문제 21-2-1
////char str[1000];
//
//// 문제 21-2-2
////char str1[20];
////char str2[20];
////char str3[40];
////
////void Clear(char* str)
////{
////    int len = sizeof(str) / sizeof(char);
////    str[len - 2] = '\0';
////    return;
////}
//
//// 문제 21-2-3
//char str1[1000];
//char str1_name[1000];
//char str1_age[1000];
//char str2[1000];
//char str2_name[1000];
//char str2_age[1000];
//
//int main(void)
//{
//    // 문제 21-2-1
//    /*fgets(str, sizeof(str), stdin);
//
//    int cnt = 0;
//    int sum = 0;
//    while (str[cnt] != '\0')
//    {
//        if (str[cnt] >= '1' && str[cnt] <= '9')
//        {
//            sum += str[cnt] - '0';
//        }
//        cnt++;
//    }
//
//    printf("%d", sum);*/
//
//    // 문제 21-2-2
//    /*fgets(str1, sizeof(str1), stdin);
//    Clear(str1);
//    fgets(str2, sizeof(str1), stdin);
//    Clear(str2);
//
//    strcat(str3, str1);
//    strcat(str3, str2);
//
//    printf("%s\n", str3);*/
//
//    // 문제 21-2-3
//    fgets(str1, sizeof(str1), stdin);
//    fgets(str2, sizeof(str2), stdin);
//
//    int len1 = sizeof(str1) / sizeof(char);
//    int len2 = sizeof(str2) / sizeof(char);
//
//    int flag1 = 1;
//    int cnt1 = 0;
//    for (int i = 0; i < len1; i++)
//    {
//        if (str1[i] == ' ')
//        {
//            flag1 = 0;
//            cnt1 = 0;
//            continue;
//        }
//
//        if (flag1)
//        {
//            str1_name[cnt1++] = str1[i];
//        }
//        else
//        {
//            str1_age[cnt1++] = str1[i];
//        }
//    }
//
//    int flag2 = 1;
//    int cnt2 = 0;
//    for (int i = 0; i < len2; i++)
//    {
//        if (str2[i] == ' ')
//        {
//            flag2 = 0;
//            cnt2 = 0;
//            continue;
//        }
//
//        if (flag2)
//        {
//            str2_name[cnt2++] = str2[i];
//        }
//        else
//        {
//            str2_age[cnt2++] = str2[i];
//        }
//    }
//
//    if (strcmp(str1_name, str2_name) == 0)
//    {
//        if (strcmp(str1_age, str2_age) == 0)
//        {
//            printf("이름과 나이가 일치합니다.\n");
//        }
//        else
//        {
//            printf("이름은 일치하지만 나이는 일치하지 않습니다.\n");
//        }
//    }
//    else
//    {
//        if (strcmp(str1_age, str2_age) == 0)
//        {
//            printf("이름은 일치하지 않지만 나이는 일치합니다\n");
//        }
//        else
//        {
//            printf("이름과 나이가 일치하지 않습니다.\n");
//        }
//    }
//    
//
//    return 0;
//}