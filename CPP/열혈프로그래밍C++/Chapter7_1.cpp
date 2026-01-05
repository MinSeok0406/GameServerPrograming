//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <cstring>
//using namespace std;
//
//// 문제 7-1-1
////class Car
////{
////private:
////    int gasolineGauge;
////
////public:
////    Car(int gas) : gasolineGauge(gas)
////    {
////        
////    }
////
////    int GetGasGauge() const
////    {
////        return gasolineGauge;
////    }
////};
////
////class HybridCar : public Car
////{
////private:
////    int electricGauge;
////
////public:
////    HybridCar(int gas, int elec) : Car(gas), electricGauge(elec)
////    {
////        
////    }
////
////    int GetElecGauge() const
////    {
////        return electricGauge;
////    }
////};
////
////class HybridWaterCar : public HybridCar
////{
////private:
////    int waterGauge;
////
////public:
////    HybridWaterCar(int gas, int elec, int water) : HybridCar(gas, elec), waterGauge(water)
////    {
////        
////    }
////
////    void ShowCurrentGauge()
////    {
////        cout << "잔여 가솔린: " << GetGasGauge() << "\n";
////        cout << "잔여 전기량: " << GetElecGauge() << "\n";
////        cout << "잔여 워터량: " << waterGauge << "\n";
////    }
////};
//
//// 문제 7-1-2
//class MyFriendInfo
//{
//private:
//    char* name;
//    int age;
//
//public:
//    MyFriendInfo(const char* n, int a) : age(a)
//    {
//        cout << "MyFriendInfo 생성자 생성" << "\n";
//        name = new char[strlen(n) + 1];
//        strcpy(name, n);
//    }
//
//    ~MyFriendInfo()
//    {
//        cout << "MyFriendInfo 소멸자 생성" << "\n";
//        delete[] name;
//    }
//
//    void ShowMyFriendInfo()
//    {
//        cout << "이름: " << name << "\n";
//        cout << "나이: " << age << "\n";
//    }
//};
//
//class MyFriendDetailInfo : public MyFriendInfo
//{
//private:
//    char* addr;
//    char* phone;
//
//public:
//    MyFriendDetailInfo(const char* n, int a, const char* ad, const char* ph) : MyFriendInfo(n, a)
//    {
//        cout << "MyFriendDetailInfo 생성자 생성" << "\n";
//        addr = new char[strlen(ad) + 1];
//        phone = new char[strlen(ph) + 1];
//        strcpy(addr, ad);
//        strcpy(phone, ph);
//    }
//
//    ~MyFriendDetailInfo()
//    {
//        cout << "MyFriendDetailInfo 소멸자 생성" << "\n";
//        delete[] addr;
//        delete[] phone;
//    }
//
//    void ShowMyFriendDetailInfo()
//    {
//        ShowMyFriendInfo();
//        cout << "주소: " << addr << "\n";
//        cout << "전화: " << phone << "\n\n";
//    }
//};
//
//int main()
//{
//    // 문제 7-1-1
//    /*HybridWaterCar hybrid_water_car = {10, 20, 30};
//    hybrid_water_car.ShowCurrentGauge();*/
//
//    // 문제 7-1-2
//    MyFriendDetailInfo mfdi = {"장민석", 26, "경기도", "010-000-0000"};
//    mfdi.ShowMyFriendDetailInfo();
//    
//
//    return 0;
//}