//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <cstring>
//using namespace std;
//
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
//    void ShowPosition() const
//    {
//        cout << "[" << xpos << ", " << ypos << "]" << "\n";
//    }
//
//    // 문제 10-2-1
//    Point operator-()
//    {
//        Point pos;
//        pos.xpos = -xpos;
//        pos.ypos = -ypos;
//        return pos;
//    }
//
//    // 문제 10-2-2
//    friend Point operator~(const Point& ref);
//};
//
//Point operator~(const Point& ref)
//{
//    Point pos;
//    pos.xpos = ref.ypos;
//    pos.ypos = ref.xpos;
//    return pos;
//}
//
//int main()
//{
//    // 문제 10-2-1
//    /*Point pos1(3, 4);
//    Point pos2 = -pos1;
//
//    pos2.ShowPosition();*/
//
//    Point pos1(5, -10);
//    Point pos2 = ~pos1;
//
//    pos2.ShowPosition();
//
//    return 0;
//}
