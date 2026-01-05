//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//// 문제 22-2-1
//struct employee
//{
//    char name[20];
//    char ID[20];
//    int price;
//};
//
//int main(void)
//{
//    // 문제 22-2-1
//    struct employee em[3];
//
//    for (int i = 0; i < 3; i++)
//    {
//        printf("이름 : ");
//        scanf_s("%s", &em[i].name, sizeof(em[i].name));
//        printf("주민등록번호 : ");
//        scanf_s("%s", &em[i].ID, sizeof(em[i].ID));
//        printf("급여 정보 : ");
//        scanf_s("%d", &em[i].price);
//    }
//
//    for (int i = 0; i < 3; i++)
//    {
//        printf("이름 : %s\n", em[i].name);
//        printf("주민등록번호 : %s\n", em[i].ID);
//        printf("급여 정보 : %d\n\n", em[i].price);
//    }
//
//    return 0;
//}