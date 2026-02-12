//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <time.h>
//
//// 도전 1
////int arr[4][4] = {
////    {1, 2, 3, 4},
////    {5, 6, 7, 8},
////    {9, 10, 11, 12},
////    {13, 14, 15, 16}
////};
////
////void Rotate(int arr[][4], int n)
////{
////    int temp[4][4];
////
////    for (int i = 0; i < n; i++)
////    {
////        for (int j = 0; j < n; j++)
////        {
////            // 시계방향
////            temp[i][j] = arr[n - j - 1][i];
////
////            // 반시계방향
////            //temp[i][j] = arr[j][n - i - 1];
////        }
////    }
////
////    for (int i = 0; i < n; i++)
////    {
////        for (int j = 0; j < n; j++)
////        {
////            arr[i][j] = temp[i][j];
////        }
////    }
////}
//
//// 도전 2
////int arr[1000][1000];
////int n;
////int dy[4] = {0, 1, 0, -1};
////int dx[4] = {1, 0, -1, 0};
////
////void CreateSnail(int y, int x)
////{
////    int k = 0, cnt = 1;
////    arr[y][x] = cnt++;
////
////    while (cnt <= n * n)
////    {
////        int ny = y + dy[k];
////        int nx = x + dx[k];
////
////        if (ny < 0 || ny >= n || nx < 0 || nx >= n || arr[ny][nx])
////        {
////            k = (k + 1) % 4;
////            continue;
////        }
////
////        arr[ny][nx] = cnt++;
////        y = ny;
////        x = nx;
////    }
////}
//
//// 도전 6
//int com[3];
//int player[3];
//void CreateNumber(int num1, int num2, int num3)
//{
//    com[0] = num1;
//    com[1] = num2;
//    com[2] = num3;
//}
//
//void CmpNumber(int num1, int num2, int num3, int* st, int* ba)
//{
//    player[0] = num1;
//    player[1] = num2;
//    player[2] = num3;
//
//    for (int i = 0; i < 3; i++)
//    {
//        if (player[i] == com[i])
//        {
//            (*st)++;
//        }
//    }
//
//    for (int i = 0; i < 3; i++)
//    {
//        for (int j = 0; j < 3; j++)
//        {
//            if (i == j)
//            {
//                continue;
//            }
//
//            if (player[i] == com[j])
//            {
//                (*ba)++;
//            }
//        }
//    }
//}
//
//int main(void)
//{
//    // 도전 1
//    /*int size = sizeof(arr) / sizeof(arr[0]);
//    Rotate(arr, size);
//    Rotate(arr, size);
//    Rotate(arr, size);
//
//    for (int i = 0; i < size; i++)
//    {
//        for (int j = 0; j < size; j++)
//        {
//            printf("%d ", arr[i][j]);
//        }
//        printf("\n");
//    }*/
//
//    // 도전 2
//    /*printf("숫자를 입력하시오 : ");
//    scanf_s("%d", &n);
//    CreateSnail(0, 0);
//
//    for (int i = 0; i < n; i++)
//    {
//        for (int j = 0; j < n; j++)
//        {
//            printf("%d ", arr[i][j]);
//        }
//        printf("\n");
//    }*/
//
//    // 도전 3
//    /*int i;
//    printf("난수의 범위: 0부터 99까지\n");
//    for (i = 0; i < 5; i++)
//    {
//        printf("난수의 출력: %d\n", rand() % 100);
//    }*/
//
//    // 도전 4
//    /*srand((unsigned int)time(NULL));
//    printf("주사위 1의 결과 %d\n", (rand() % 6) + 1);
//    printf("주사위 2의 결과 %d\n", (rand() % 6) + 1);*/
//    
//    // 도전 5
//    /*srand((unsigned int)time(NULL));
//    int win = 0, draw = 0;
//    while (1)
//    {
//        int n;
//        printf("바위는 1, 가위는 2, 보는 3: ");
//        scanf_s("%d", &n);
//
//        int com = (rand() % 3) + 1;
//
//        if (n == 1)
//        {
//            if (com == 1)
//            {
//                printf("당신은 바위 선택, 컴퓨터는 바위 선택, 비겼습니다!\n");
//                draw++;
//            }
//            else if (com == 2)
//            {
//                printf("당신은 바위 선택, 컴퓨터는 가위 선택, 이겼습니다!\n");
//                win++;
//            }
//            else if (com == 3)
//            {
//                printf("당신은 바위 선택, 컴퓨터는 보 선택, 졌습니다!\n");
//                break;
//            }
//        }
//        else if (n == 2)
//        {
//            if (com == 1)
//            {
//                printf("당신은 가위 선택, 컴퓨터는 바위 선택, 졌습니다!\n");
//                break;
//            }
//            else if (com == 2)
//            {
//                printf("당신은 가위 선택, 컴퓨터는 가위 선택, 비겼습니다!\n");
//                draw++;
//            }
//            else if (com == 3)
//            {
//                printf("당신은 가위 선택, 컴퓨터는 보 선택, 이겼습니다!\n");
//                win++;
//            }
//        }
//        else if (n == 3)
//        {
//            if (com == 1)
//            {
//                printf("당신은 보 선택, 컴퓨터는 바위 선택, 이겼습니다!\n");
//                win++;
//            }
//            else if (com == 2)
//            {
//                printf("당신은 보 선택, 컴퓨터는 가위 선택, 졌습니다!\n");
//                break;
//            }
//            else if (com == 3)
//            {
//                printf("당신은 보 선택, 컴퓨터는 보 선택, 비겼습니다!\n");
//                draw++;
//            }
//        }
//    }
//    printf("\n\n");
//    printf("게임의 결과 : %d승, %d무\n", win, draw);*/
//
//    // 도전 6
//    srand((unsigned int)time(NULL));
//    printf("Start Game!\n");
//    int a = rand() % 9 + 1;
//    int b = rand() % 9 + 1;
//    int c = rand() % 9 + 1;
//    CreateNumber(a, b, c);
//
//    int cnt = 1;
//    while (1)
//    {
//        int num1, num2, num3;
//        int strike = 0, ball = 0;
//        printf("3개의 숫자 선택: ");
//        scanf_s("%d %d %d", &num1, &num2, &num3);
//        CmpNumber(num1, num2, num3, &strike, &ball);
//        printf("%d번째 도전 결과: %dstrike, %dball!!\n", cnt, strike, ball);
//
//        if (strike == 3)
//        {
//            break;
//        }
//
//        cnt++;
//    }
//
//    return 0;
//}