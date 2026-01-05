//#include <iostream>
//using namespace std;
//
//typedef struct Point
//{
//    int xpos;
//    int ypos;
//} Point;
//
//Point& PointAdder(const Point& pos1, const Point& pos2)
//{
//    Point* pos = new Point;
//    pos->xpos = pos1.xpos + pos2.xpos;
//    pos->ypos = pos1.ypos + pos2.ypos;
//    return *pos;
//}
//
//int main()
//{
//    int num = 5;
//    int* ptr = &num;
//    int& ref = *ptr;
//
//    Point* pos1 = new Point;
//    Point* pos2 = new Point;
//    pos1->xpos = 1;
//    pos1->ypos = 1;
//    pos2->xpos = 3;
//    pos2->ypos = 3;
//
//    Point& pos = PointAdder(*pos1, *pos2);
//
//    cout << pos.xpos << " " << pos.ypos << "\n";
//
//    delete pos1;
//    delete pos2;
//    delete& pos;
//
//    return 0;
//}