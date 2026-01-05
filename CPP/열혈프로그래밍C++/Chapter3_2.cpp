//#include <iostream>
//using namespace std;
//
//// ¹®Á¦ 3-2-1
//class Calculator
//{
//public:
//    Calculator()
//    {
//        
//    }
//
//    void Init()
//    {
//        add = 0;
//        sub = 0;
//        mul = 0;
//        div = 0;
//    }
//
//    double Add(double a, double b)
//    {
//        add++;
//        return a + b;
//    }
//
//    double Min(double a, double b)
//    {
//        sub++;
//        return a - b;
//    }
//
//    double Mul(double a, double b)
//    {
//        mul++;
//        return a * b;
//    }
//
//    double Div(double a, double b)
//    {
//        div++;
//        return a / b;
//    }
//
//    void ShowOpCount()
//    {
//        cout << "µ¡¼À: " << add << " »¬¼À: " << sub << " °ö¼À: " << mul << " ³ª´°¼À: " << div << "\n";
//    }
//
//private:
//    int add;
//    int sub;
//    int mul;
//    int div;
//};
//
//// ¹®Á¦ 3-2-2
//class Printer
//{
//public:
//    void SetString(const char* arr)
//    {
//        strcpy_s(name, arr);
//    }
//
//    void ShowString()
//    {
//        cout << name << "\n";
//    }
//
//private:
//    char name[100];
//};
//
//int main()
//{
//    // ¹®Á¦ 3-2-1
//    Calculator cal;
//    cal.Init();
//    cout << "3.2 + 2.4 = " << cal.Add(3.2, 2.4) << "\n";
//    cout << "3.5 / 1.7 = " << cal.Div(3.5, 1.7) << "\n";
//    cout << "2.2 - 1.5 = " << cal.Min(2.2, 1.5) << "\n";
//    cout << "4.9 / 1.2 = " << cal.Div(4.9, 1.2) << "\n";
//    cal.ShowOpCount();
//
//    // ¹®Á¦ 3-2-2
//    Printer pnt;
//    pnt.SetString("Hello World!!");
//    pnt.ShowString();
//
//    pnt.SetString("I love C++");
//    pnt.ShowString();
//
//    return 0;
//}