//#include <stdio.h>
//#include <conio.h>
//#include <stdlib.h>
//#include <string.h>
//
//int main(void)
//{
//    int nList[5];
//
//    for (int i = 0; i < 5; i++)
//    {
//        scanf_s("%d", &nList[i]);
//    }
//
//    for (int i = 0; i < 4; i++)
//    {
//        int idx = i;
//        for (int j = i + 1; j < 5; j++)
//        {
//            if (nList[idx] >= nList[j])
//            {
//                idx = j;
//            }
//        }
//
//        if (idx != i)
//        {
//            int temp = nList[idx];
//            nList[idx] = nList[i];
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