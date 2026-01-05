//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <cstring>
//using namespace std;
//
//// 문제 4-3-2
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
//        name = (char*)malloc(sizeof(char) * len);
//        strcpy(name, n);
//
//        len = strlen(cn);
//        companyName = (char*)malloc(sizeof(char) * len);
//        strcpy(companyName, cn);
//
//        len = strlen(nb);
//        number = (char*)malloc(sizeof(char) * len);
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
//    ~NameCard()
//    {
//        free(name);
//        free(companyName);
//        free(number);
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
//    NameCard manSENIOR("Hong", "OrangeEng", "010-3333-4444", COMP_POS::SENIOR);
//    NameCard manAssist("Kim", "SoGoodComp", "010-5555-6666", COMP_POS::ASSIST);
//
//    manClerk.ShowNameCardInfo();
//    manSENIOR.ShowNameCardInfo();
//    manAssist.ShowNameCardInfo();
//
//    return 0;
//}