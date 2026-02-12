//#include <stdio.h>
//
//// 도전2
////void GuGuDan(int a, int b)
////{
////    int start = (a > b) ? b : a;
////    int end = (a > b) ? a : b;
////
////    for (int i = start; i <= end; i++)
////    {
////        for (int j = 1; j <= 9; j++)
////        {
////            printf("%d * %d = %d\n", i, j, i * j);
////        }
////    }
////}
//
//// 도전3
////int GCD(int num1, int num2)
////{
////    if (num2 == 0)
////    {
////        return num1;
////    }
////
////    return GCD(num2, num1 % num2);
////}
//
//// 도전4
////int visited[101][101][101];
////
////void go(int cream, int shrimp, int coke, int sum)
////{
////    if (sum < 0)
////    {
////        return;
////    }
////
////    if (sum == 0)
////    {
////        if (cream == 0 || shrimp == 0 || coke == 0)
////        {
////            return;
////        }
////
////        if (visited[cream][shrimp][coke])
////        {
////            return;
////        }
////        
////        visited[cream][shrimp][coke] = 1;
////        printf("크림빵: %d, 새우깡: %d, 콜라: %d\n", cream, shrimp, coke);
////
////        return;
////    }
////
////    go(cream, shrimp + 1, coke, sum - 700);
////    go(cream, shrimp, coke + 1, sum - 400);
////    go(cream + 1, shrimp, coke, sum - 500);
////
////    return;
////}
//
//// 도전5
////int cnt = 10;
////void isPrime(int num)
////{
////    for (int i = 2; i < num; i++)
////    {
////        if (num % i == 0)
////        {
////            return;
////        }
////    }
////
////    printf("%d ", num);
////    cnt--;
////}
//
//// 도전8
//int Recur(int num)
//{
//    if (num <= 0)
//    {
//        return 1;
//    }
//
//    return 2 * Recur(num - 1);
//}
//
//int main(void)
//{
//    // 도전1
//    /*int num;
//    scanf_s("%d", &num);
//    printf("%x\n", num);*/
//
//    // 도전2
//    /*int num1, num2;
//    scanf_s("%d %d", &num1, &num2);
//    GuGuDan(num1, num2);*/
//
//    // 도전3
//    /*int num1, num2;
//    scanf_s("%d %d", &num1, &num2);
//    printf("%d\n", GCD(num1, num2));*/
//
//    // 도전4
//    /*int money;
//    scanf_s("%d", &money);
//    go(0, 0, 0, money);*/
//
//    // 도전5
//    /*int num = 2;
//    while (cnt)
//    {
//        isPrime(num);
//        num++;
//    }*/
//
//    // 도전6
//    /*int t;
//    printf("초(second) 입력: ");
//    scanf_s("%d", &t);
//    
//    int h = t / 3600;
//    int m = (t % 3600) / 60;
//    int s = t % 60;
//
//    printf("[h:%d, m:%d, s:%d]\n", h, m, s);*/
//
//    // 도전7
//    /*int num;
//    int cnt = 0;
//    scanf_s("%d", &num);
//
//    while (num > 1)
//    {
//        num /= 2;
//        cnt++;
//    }
//
//    printf("%d", cnt);*/
//
//    // 도전8
//    int num;
//    printf("정수 입력 : ");
//    scanf_s("%d", &num);
//
//    printf("2의 %d승은 %d\n", num, Recur(num));
//
//    return 0;
//}