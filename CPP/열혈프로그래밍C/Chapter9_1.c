//#include <stdio.h>
//
//// ¹®Á¦ 9-1-1
////int OutputMax(int a, int b)
////{
////    if (a > b)
////    {
////        return a;
////    }
////    else
////    {
////        return b;
////    }
////}
////
////int OutputMin(int a, int b)
////{
////    if (a < b)
////    {
////        return a;
////    }
////    else
////    {
////        return b;
////    }
////}
////
////void PrintMaxMin(int a, int b, int c)
////{
////    int max_num = OutputMax(OutputMax(a, b), c);
////    int min_num = OutputMin(OutputMin(a, b), c);
////
////    printf("ÃÖ´ë°ª: %d | ÃÖ¼Ò°ª: %d\n", max_num, min_num);
////}
//
//// ¹®Á¦ 9-1-2
////double CelToFah(double cel)
////{
////    return cel * 1.8 + 32;
////}
////
////double FahToCel(double fah)
////{
////    return (fah - 32) / 1.8;
////}
//
//// ¹®Á¦ 9-1-3
//void Fibo(int n)
//{
//    if (n >= 1)
//    {
//        printf("0 ");
//    }
//
//    if (n >= 2)
//    {
//        printf("1 ");
//    }
//
//    int pred = 0;
//    int pre = 1;
//    int sum = 0;
//    for (int i = 3; i <= n; i++)
//    {
//        sum = pre + pred;
//        printf("%d ", sum);
//        pred = pre;
//        pre = sum;
//    }
//}
//
//int main(void)
//{
//    // ¹®Á¦ 9-1-1
//    /*int a = 10;
//    int b = 20;
//    int c = 15;
//
//    PrintMaxMin(a, b, c);*/
//
//    // ¹®Á¦ 9-1-2
//    /*double temp = 27.3;
//    printf("¼·¾¾ -> È­¾¾ : %lf\n", CelToFah(temp));
//    printf("È­¾¾ -> ¼·¾¾ : %lf\n", FahToCel(CelToFah(temp)));*/
//
//    // ¹®Á¦ 9-1-3
//    Fibo(8);
//
//    return 0;
//}