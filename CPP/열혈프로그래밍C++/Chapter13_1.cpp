//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <cstring>
//using namespace std;
//
//// 巩力 13-1-1
////class Point
////{
////private:
////    int xpos, ypos;
////
////public:
////    Point(int x = 0, int y = 0) : xpos(x), ypos(y)
////    {
////        
////    }
////
////    void ShowPosition() const
////    {
////        cout << '[' << xpos << ", " << ypos << ']' << "\n";
////    }
////
////    template <typename T>
////    friend void SwapData(T& a);
////};
////
////template <typename T>
////void SwapData(T& a)
////{
////    int temp = a.xpos;
////    a.xpos = a.ypos;
////    a.ypos = temp;
////}
//
//// 巩力 13-1-2
//template <typename T>
//T SumArray(T arr[], int len)
//{
//    T sum = 0;
//    for (int i = 0; i < len; i++)
//    {
//        sum += arr[i];
//    }
//
//    return sum;
//}
//
//int main()
//{
//    // 巩力 13-1-1
//    /*Point p(5, 4);
//
//    p.ShowPosition();
//    SwapData<Point>(p);
//    p.ShowPosition();*/
//
//    // 巩力 13-1-2
//    int iarr[5] = {1, 2, 3, 4, 5};
//    double darr[5] = {1.1, 2.2, 3.3, 4.4, 5.5};
//
//    cout << SumArray<int>(iarr, 5) << "\n";
//    cout << SumArray<double>(darr, 5) << "\n";
//
//    return 0;
//}