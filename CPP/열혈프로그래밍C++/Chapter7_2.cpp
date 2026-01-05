//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <cstring>
//using namespace std;
//
//// 문제 7-2-1
////class Retangle
////{
////private:
////    int height;
////    int width;
////
////public:
////    Retangle(int h, int w) : height(h), width(w)
////    {
////        
////    }
////
////    void ShowAreaInfo() const
////    {
////        cout << "면적: " << height * width << "\n";
////    }
////};
////
////class Square : public Retangle
////{
////public:
////    Square(int dis) : Retangle(dis, dis)
////    {
////        
////    }
////};
//
//// 문제 7-2-2
//class Book
//{
//private:
//    char* title;
//    char* isbn;
//    int price;
//
//public:
//    Book(const char* t, const char* is, int cost) : price(cost)
//    {
//        title = new char[strlen(t) + 1];
//        isbn = new char[strlen(is) + 1];
//        strcpy(title, t);
//        strcpy(isbn, is);
//    }
//
//    ~Book()
//    {
//        delete[] title;
//        delete[] isbn;
//    }
//
//    void ShowBookInfo() const
//    {
//        cout << "제목: " << title << "\n";
//        cout << "ISBN: " << isbn << "\n";
//        cout << "가격: " << price << "\n";
//    }
//};
//
//class EBook : public Book
//{
//private:
//    char* DRMKey;
//
//public:
//    EBook(const char* t, const char* is, int cost, const char* key) : Book(t, is, cost)
//    {
//        DRMKey = new char[strlen(key) + 1];
//        strcpy(DRMKey, key);
//    }
//
//    ~EBook()
//    {
//        delete[] DRMKey;
//    }
//
//    void ShowEBookInfo() const
//    {
//        ShowBookInfo();
//        cout << "인증키: " << DRMKey << "\n";
//    }
//};
//
//int main()
//{
//    // 문제 7-2-1
//    /*Retangle rec(4, 3);
//    rec.ShowAreaInfo();
//
//    Square sqr(7);
//    sqr.ShowAreaInfo();*/
//
//    // 문제 7-2-2
//    Book book("좋은 C++", "555-12345-890-0", 20000);
//    book.ShowBookInfo();
//    cout << "\n";
//    EBook ebook("좋은 C++ ebook", "555-12345-890-1", 10000, "fdx9w018kiw");
//    ebook.ShowEBookInfo();
//
//    return 0;
//}