//#include <stdio.h>
//#include <conio.h>
//#include <stdlib.h>
//#include <string.h>
//
//
//
//int main(void)
//{
//    int nList[5];
//    int nMax = -1;
//
//    for (int i = 0; i < 5; i++)
//    {
//        scanf_s("%d", &nList[i]);
//    }
//
//    for (int i = 0; i < 5; i++)
//    {
//        if (nMax <= nList[i])
//        {
//            nMax = nList[i];
//        }
//    }
//
//    printf("MAX: %d\n", nMax);
//
//    printf("-------------------\n");
//
//    for (int i = 1; i < 5; i++)
//    {
//        if (nList[0] >= nList[i])
//        {
//            int temp = nList[0];
//            nList[0] = nList[i];
//            nList[i] = temp;
//        }
//    }
//
//    for (int i = 0; i < 5; i++)
//    {
//        printf("%d ", nList[i]);
//    }
//
//    return 0;
//}