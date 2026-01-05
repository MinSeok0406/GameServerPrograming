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
//    friend ostream& operator<<(ostream&, const Point&);
//    friend istream& operator>>(istream&, Point&);
//};
//
//ostream& operator<<(ostream& os, const Point& pos)
//{
//    os << "[" << pos.xpos << ", " << pos.ypos << "]" << "\n";
//    return os;
//}
//
//// 문제 10-3-1
//istream& operator>>(istream& os, Point& pos)
//{
//    os >> pos.xpos >> pos.ypos;
//    return os;
//}
//
//int main()
//{
//    Point pos1;
//    cout << "x, y 좌표 순으로 입력: ";
//    cin >> pos1;
//    cout << pos1;
//
//    Point pos2;
//    cout << "x, y 좌표 순으로 입력: ";
//    cin >> pos2;
//    cout << pos2;
//
//
//    return 0;
//}
