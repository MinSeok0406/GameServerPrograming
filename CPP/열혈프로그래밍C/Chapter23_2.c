//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//// 문제 23-2-1
//typedef struct point
//{
//    int xpos;
//    int ypos;
//} Point;
//
//typedef struct rectangle
//{
//    Point lupos;
//    Point rdpos;
//} Rectangle;
//
//int Width(Rectangle rec)
//{
//    int height = abs(rec.lupos.ypos - rec.rdpos.ypos);
//    int width = abs(rec.lupos.xpos - rec.rdpos.xpos);
//
//    return height * width;
//}
//
//int main(void)
//{
//    // 문제 23-2-1
//    Rectangle rec = {{50, 10}, {100, 100}};
//    printf("넓이: %d\n", Width(rec));
//
//    return 0;
//}