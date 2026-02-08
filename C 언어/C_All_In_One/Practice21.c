//#include <stdio.h>
//#include <conio.h>
//#include <stdlib.h>
//#include <string.h>
//
//const int dy[4] = { 0, 1, 0, -1 };
//const int dx[4] = { 1, 0, -1, 0 };
//int adj[5][5];
//int visited[5][5];
//
//int main(void)
//{
//    int nList[5][5];
//
//    for (int i = 0; i < 5; i++)
//    {
//        if (i % 2 == 0)
//        {
//            for (int j = 0; j < 5; j++)
//            {
//                nList[i][j] = j + i * 5;
//                nList[i][j]++;
//            }
//        }
//        else
//        {
//            int cnt = 0;
//            for (int j = 4; j >= 0; j--)
//            {
//                nList[i][cnt] = j + i * 5;
//                nList[i][cnt++]++;
//            }
//        }
//    }
//
//    for (int i = 0; i < 5; i++)
//    {
//        for (int j = 0; j < 5; j++)
//        {
//            printf("%d ", nList[i][j]);
//        }
//        printf("\n");
//    }
//
//    printf("\n");
//    //------------------------------------------
//    printf("\n");
//
//    int y = 0, x = 0, dir = 0, cnt = 0;
//    adj[y][x] = 1, visited[y][x] = 1;
//    while (cnt < (5 * 5) - 1)
//    {
//        int ny = y + dy[dir];
//        int nx = x + dx[dir];
//
//        if (ny < 0 || ny >= 5 || nx < 0 || nx >= 5 || visited[ny][nx])
//        {
//            dir = (dir + 1) % 4;
//            continue;
//        }
//
//        adj[ny][nx] = adj[y][x] + 1;
//        visited[ny][nx] = 1;
//
//        y = ny;
//        x = nx;
//        cnt++;
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