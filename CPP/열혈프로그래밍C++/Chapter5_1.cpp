//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <cstring>
//using namespace std;
//
//// 문제 5-1-1
//
//enum COMP_POS
//{
//    CLERK,
//    SENIOR,
//    ASSIST,
//    MANAGER
//};
//
//class NameCard
//{
//private:
//    char* name;
//    char* companyName;
//    char* number;
//    int rank;
//
//public:
//    NameCard(const char* n, const char* cn, const char* nb, int rk)
//    {
//        int len = strlen(n);
//        name = new char[len + 1];
//        strcpy(name, n);
//
//        len = strlen(cn);
//        companyName = new char[len + 1];
//        strcpy(companyName, cn);
//
//        len = strlen(nb);
//        number = new char[len + 1];
//        strcpy(number, nb);
//
//        if (rk == COMP_POS::CLERK)
//        {
//            rank = COMP_POS::CLERK;
//        }
//        else if (rk == COMP_POS::SENIOR)
//        {
//            rank = COMP_POS::SENIOR;
//        }
//        else if (rk == COMP_POS::ASSIST)
//        {
//            rank = COMP_POS::ASSIST;
//        }
//        else if (rk == COMP_POS::MANAGER)
//        {
//            rank = COMP_POS::MANAGER;
//        }
//    }
//
//    NameCard(const NameCard& nc)
//    {
//        int len = strlen(nc.name);
//        name = new char[len + 1];
//        strcpy(name, nc.name);
//
//        len = strlen(nc.companyName);
//        companyName = new char[len + 1];
//        strcpy(companyName, nc.companyName);
//
//        len = strlen(nc.number);
//        number = new char[len + 1];
//        strcpy(number, nc.number);
//
//        rank = nc.rank;
//    }
//
//    ~NameCard()
//    {
//        delete[] name;
//        delete[] companyName;
//        delete[] number;
//    }
//
//    void ShowNameCardInfo() const
//    {
//        cout << "이름: " << name << "\n";
//        cout << "회사: " << companyName << "\n";
//        cout << "전화번호: " << number << "\n";
//
//        if (rank == COMP_POS::CLERK)
//        {
//            cout << "직급: 사원" << "\n";
//        }
//        else if (rank == COMP_POS::SENIOR)
//        {
//            cout << "직급: 주임" << "\n";
//        }
//        else if (rank == COMP_POS::ASSIST)
//        {
//            cout << "직급: 대리" << "\n";
//        }
//        else if (rank == COMP_POS::MANAGER)
//        {
//            cout << "직급: 과장" << "\n";
//        }
//
//        cout << "\n";
//    }
//};
//
//int main()
//{
//    NameCard manClerk("Lee", "ABCEng", "010-1111-2222", COMP_POS::CLERK);
//    NameCard copy1 = manClerk;
//    NameCard manSENIOR("Hong", "OrangeEng", "010-3333-4444", COMP_POS::SENIOR);
//    NameCard copy2 = manSENIOR;
//
//    copy1.ShowNameCardInfo();
//    copy2.ShowNameCardInfo();
//
//    return 0;
//}