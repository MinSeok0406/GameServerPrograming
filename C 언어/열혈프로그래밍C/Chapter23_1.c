//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//// 문제 23-1-1
//typedef struct point
//{
//    int xpos;
//    int ypos;
//} Point;
//
//void SwapPoint(Point* pos1, Point* pos2)
//{
//    int xtemp = pos1->xpos;
//    pos1->xpos = pos2->xpos;
//    pos2->xpos = xtemp;
//
//    int ytemp = pos1->ypos;
//    pos1->ypos = pos2->ypos;
//    pos2->ypos = ytemp;
//    
//    return;
//}
//
//int main(void)
//{
//    // 문제 23-1-1
//    Point pos1 = {2, 4};
//    Point pos2 = {-5, -1};
//
//    printf("변경 전\n");
//    printf("%d %d || %d %d\n\n", pos1.xpos, pos1.ypos, pos2.xpos, pos2.ypos);
//    SwapPoint(&pos1, &pos2);
//    printf("변경 후\n");
//    printf("%d %d || %d %d\n\n", pos1.xpos, pos1.ypos, pos2.xpos, pos2.ypos);
//
//    return 0;
//}