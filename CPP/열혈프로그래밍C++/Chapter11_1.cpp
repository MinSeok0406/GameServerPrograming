//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <cstring>
//using namespace std;
//
//// 문제 11-1-1
////class Gun
////{
////private:
////    int bullet;
////
////public:
////    Gun(int bnum) : bullet(bnum)
////    {
////        
////    }
////
////    void Shot()
////    {
////        cout << "BBANG!" << "\n";
////        bullet--;
////    }
////};
////
////class Police
////{
////private:
////    int handcuffs;
////    Gun* pistol;
////
////public:
////    Police(int bnum, int bcuff) : handcuffs(bcuff)
////    {
////        if (bnum > 0)
////        {
////            pistol = new Gun(bnum);
////        }
////        else
////        {
////            pistol = NULL;
////        }
////    }
////
////    Police(const Police& p)
////    {
////        handcuffs = p.handcuffs;
////        
////        if (p.pistol == NULL)
////        {
////            pistol = NULL;
////        }
////        else
////        {
////            pistol = new Gun(*(p.pistol));
////        }
////    }
////
////    Police& operator=(const Police& p)
////    {
////        if (pistol != NULL)
////        {
////            delete pistol;
////        }
////        
////        if (p.pistol != NULL)
////        {
////            pistol = new Gun(*(p.pistol));
////        }
////        else
////        {
////            pistol = NULL;
////        }
////
////        handcuffs = p.handcuffs;
////        return *this;
////    }
////
////    void PutHandcuff()
////    {
////        cout << "SNAP!" << "\n";
////        handcuffs--;
////    }
////
////    void Shot()
////    {
////        if (pistol == NULL)
////        {
////            cout << "Hut BBANG!" << "\n";
////        }
////        else
////        {
////            pistol->Shot();
////        }
////    }
////
////    ~Police()
////    {
////        if (pistol == NULL)
////        {
////            delete pistol;
////        }
////    }
////};
//
//// 문제 11-1-2
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
//    Book(const Book& bk)
//    {
//        title = new char[strlen(bk.title) + 1];
//        isbn = new char[strlen(bk.isbn) + 1];
//        strcpy(title, bk.title);
//        strcpy(isbn, bk.isbn);
//        price = bk.price;
//    }
//
//    Book operator=(const Book& bk)
//    {
//        delete[] title;
//        delete[] isbn;
//
//        title = new char[strlen(bk.title) + 1];
//        isbn = new char[strlen(bk.isbn) + 1];
//        strcpy(title, bk.title);
//        strcpy(isbn, bk.isbn);
//        price = bk.price;
//        return *this;
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
//    EBook(const EBook& eb) : Book(eb)
//    {
//        int len = strlen(eb.DRMKey) + 1;
//        DRMKey = new char[len];
//        strcpy(DRMKey, eb.DRMKey);
//    }
//
//    EBook& operator=(const EBook& eb)
//    {
//        Book::operator=(eb);
//        delete[] DRMKey;
//        int len = strlen(eb.DRMKey) + 1;
//        DRMKey = new char[len];
//        strcpy(DRMKey, eb.DRMKey);
//        return *this;
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
//    // 문제 11-1-1
//    /*Police p1(3, 5);
//    Police p2 = p1;
//
//    Police p3(4, 6);
//    Police p4(6, 7);
//    p3 = p4;*/
//
//    // 문제 11-1-2
//    EBook eb1("HI", "is", 100, "Ke");
//    EBook eb2 = eb1;
//
//    EBook eb3("Hello", "is", 200, "ey");
//    EBook eb4("qwer", "is", 400, "ad");
//    eb4 = eb3;
//
//
//    return 0;
//}
