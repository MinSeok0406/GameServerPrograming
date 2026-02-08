//#include <stdio.h>
//#include <conio.h>
//#include <stdlib.h>
//#include <string.h>
//
//int main(void)
//{
//    int nList[3][4] = {
//        {10, 20, 30},
//        {40, 50, 60}
//    };
//
//
//    for (int i = 0; i < 2; i++)
//    {
//        int sum = 0, j;
//        for (j = 0; j < 3; j++)
//        {
//            sum += nList[i][j];
//        }
//        nList[i][j] = sum;
//    }
//
//    for (int i = 0; i < 4; i++)
//    {
//        int sum = 0, j;
//        for (j = 0; j < 2; j++)
//        {
//            sum += nList[j][i];
//        }
//        nList[j][i] = sum;
//    }
//
//    for (int i = 0; i < 3; i++)
//    {
//        for (int j = 0; j < 4; j++)
//        {
//            printf("%d ", nList[i][j]);
//        }
//        printf("\n");
//    }
//
//    printf("\n\n");
//
//    int adj[5][5];
//
//    for (int i = 0; i < 5; i++)
//    {
//        for (int j = 0; j < 5; j++)
//        {
//            adj[i][j] = j + i * 5;
//            adj[i][j]++;
//        }
//    }
//
//    for (int i = 0; i < 5; i++)
//    {
//        for (int j = 0; j < 5; j++)
//        {
//            printf("%d ", adj[i][j]);
//        }
//        printf("\n");
//    }
//
//    return 0;
//}