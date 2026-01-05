//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <cstring>
//using namespace std;
//
//// 巩力 10-1-1
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
//    friend Point operator+(const Point& pos1, const Point& pos2);
//    friend Point operator-(const Point& pos1, const Point& pos2);
//
//    // 巩力 10-1-2
//    Point& operator+=(const Point& pos)
//    {
//        this->xpos += pos.xpos;
//        this->ypos += pos.ypos;
//        return *this;
//    }
//
//    Point& operator-=(const Point& pos)
//    {
//        this->xpos -= pos.xpos;
//        this->ypos -= pos.ypos;
//        return *this;
//    }
//
//    // 巩力 10-1-3
//    friend bool operator==(const Point& pos1, const Point& pos2);
//    friend bool operator!=(const Point& pos1, const Point& pos2);
//};
//
//Point operator+(const Point& pos1, const Point& pos2)
//{
//    Point pos;
//    pos.xpos = pos1.xpos + pos2.xpos;
//    pos.ypos = pos1.ypos + pos2.ypos;
//    return pos;
//}
//
//Point operator-(const Point& pos1, const Point& pos2)
//{
//    Point pos(pos1.xpos - pos2.xpos, pos1.ypos - pos2.ypos);
//    return pos;
//}
//
//bool operator==(const Point& pos1, const Point& pos2)
//{
//    bool xflag = (pos1.xpos == pos2.xpos);
//    bool yflag = (pos1.ypos == pos2.ypos);
//
//    if (xflag && yflag)
//    {
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}
//
//bool operator!=(const Point& pos1, const Point& pos2)
//{
//    if (pos1 == pos2)
//    {
//        return false;
//    }
//    else
//    {
//        return true;
//    }
//}
//
//
//int main()
//{
//    // 巩力 10-1-1
//    /*Point pos1(3, 4);
//    Point pos2(10, 20);
//    Point pos3 = pos1 + pos2;
//    Point pos4 = pos1 - pos2;
//
//    pos3.ShowPosition();
//    pos4.ShowPosition();*/
//
//    // 巩力 10-1-2
//    /*Point pos1(3, 4);
//    Point pos2(10, 20);
//
//    Point pos3;
//
//    pos3 += pos1;
//    pos3.ShowPosition();
//    pos3 -= pos2;
//    pos3.ShowPosition();*/
//
//    // 巩力 10-1-3
//    Point pos1(5, 5);
//    Point pos2(5, 5);
//
//    bool flag = (pos1 == pos2);
//    bool xorflag = (pos1 != pos2);
//    cout << flag << "\n";
//    cout << xorflag << "\n";
//
//    return 0;
//}
