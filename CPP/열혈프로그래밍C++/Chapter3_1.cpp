//#include <iostream>
//using namespace std;
//
//struct Point
//{
//    int xpos;
//    int ypos;
//
//    void MovePos(int x, int y)
//    {
//        this->xpos += x;
//        this->ypos += y;
//    }
//
//    void AddPoint(const Point& pos)
//    {
//        this->xpos += pos.xpos;
//        this->ypos += pos.ypos;
//    }
//
//    void ShowPosition()
//    {
//        cout << "[" << this->xpos << " ," << this->ypos << "]" << "\n";
//    }
//};
//
//int main()
//{
//    Point pos1 = {12, 4};
//    Point pos2 = {20, 30};
//
//    pos1.MovePos(-7, 10);
//    pos1.ShowPosition();
//
//    pos1.AddPoint(pos2);
//    pos1.ShowPosition();
//
//    return 0;
//}