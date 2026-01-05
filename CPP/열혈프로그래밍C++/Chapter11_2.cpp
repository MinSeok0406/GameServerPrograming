//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <cstring>
//using namespace std;
//
//// 문제 11-2-1
//class Point
//{
//private:
//    int xpos, ypos;
//
//public:
//    Point(int x = 0, int y = 0) : xpos(x), ypos(y)
//    {
//        
//    }
//
//    friend ostream& operator<<(ostream& os, const Point& pos);
//    friend ostream& operator<<(ostream& os, const Point* pos);
//};
//
//ostream& operator<<(ostream& os, const Point& pos)
//{
//    os << "[" << pos.xpos << ", " << pos.ypos << "]" << "\n";
//    return os;
//}
//
//ostream& operator<<(ostream& os, const Point* pos)
//{
//    os << "[" << pos->xpos << ", " << pos->ypos << "]" << "\n";
//    return os;
//}
//
//typedef Point* POINT_PTR;
//
//class BoundCheckPointPtrArray
//{
//private:
//    POINT_PTR* arr;
//    int arrlen;
//
//    BoundCheckPointPtrArray(const BoundCheckPointPtrArray& arr) { }
//    BoundCheckPointPtrArray& operator=(const BoundCheckPointPtrArray& arr) { }
//
//public:
//    BoundCheckPointPtrArray(int len) : arrlen(len)
//    {
//        arr = new POINT_PTR[len];
//    }
//
//    POINT_PTR& operator[] (int idx)
//    {
//        if (idx < 0 || idx >= arrlen)
//        {
//            cout << "Array index out of bound exception" << "\n";
//            exit(1);
//        }
//
//        return arr[idx];
//    }
//
//    POINT_PTR operator[] (int idx) const
//    {
//        if (idx < 0 || idx >= arrlen)
//        {
//            cout << "Array index out of bound exception" << "\n";
//            exit(1);
//        }
//
//        return arr[idx];
//    }
//
//    int GetArrLen() const { return arrlen; }
//    ~BoundCheckPointPtrArray() { delete[] arr; }
//};
//
//int main()
//{
//    // 문제 11-2-1
//    BoundCheckPointPtrArray arr(3);
//    arr[0] = new Point(3, 4);
//    arr[1] = new Point(5, 6);
//    arr[2] = new Point(7, 8);
//
//    for (int i = 0; i < arr.GetArrLen(); i++)
//    {
//        cout << arr[i];
//    }
//
//    delete arr[0];
//    delete arr[1];
//    delete arr[2];
//
//    return 0;
//}