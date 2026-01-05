//#include <iostream>
//using namespace std;
//
//// ¹®Á¦ 4-2-1 & 4-3-1
//class Point
//{
//private:
//    int xpos, ypos;
//
//public:
//    Point(int x, int y) : xpos(x), ypos(y)
//    {
//        
//    }
//
//    void ShowPointInfo() const
//    {
//        cout << "[" << xpos << ", " << ypos << "]" << "\n";
//    }
//};
//
//class Circle
//{
//private:
//    int dis;
//    Point pos;
//
//public:
//    Circle(int xpos, int ypos, int d) : pos({xpos, ypos}), dis(d)
//    {
//        
//    }
//
//    void ShowCircleInfo() const
//    {
//        cout << "radius: " << dis << "\n";
//        pos.ShowPointInfo();
//    }
//};
//
//class Ring
//{
//private:
//    Circle cir1;
//    Circle cir2;
//
//public:
//    Ring(int xpos1, int ypos1, int dis1, int xpos2, int ypos2, int dis2)
//        : cir1({ xpos1, ypos1, dis1 }), cir2({ xpos2, ypos2, dis2 })
//    {
//        
//    }
//
//    void ShowRingInfo() const
//    {
//        cout << "Inner Circle Info..." << "\n";
//        cir1.ShowCircleInfo();
//        cout << "Outter Circle Info..." << "\n";
//        cir2.ShowCircleInfo();
//    }
//};
//
//int main()
//{
//    Ring ring(1, 1, 4, 2, 2, 9);
//    ring.ShowRingInfo();
//
//    return 0;
//}