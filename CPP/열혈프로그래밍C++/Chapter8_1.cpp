//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <cstring>
//using namespace std;
//
//// ¹®Á¦ 8-1-1
//enum RISK
//{
//    RISK_A,
//    RISK_B,
//    RISK_C
//};
//
//class Employee
//{
//private:
//    char name[100];
//
//public:
//    Employee(const char* name)
//    {
//        strcpy(this->name, name);
//    }
//
//    void ShowYourName() const
//    {
//        cout << "name: " << name << "\n";
//    }
//
//    virtual int GetPay() const = 0;
//    virtual void ShowSalaryInfo() const = 0;
//};
//
//class PermanentWorker : public Employee
//{
//private:
//    int salary;
//
//public:
//    PermanentWorker(const char* name, int money) : Employee(name), salary(money)
//    {
//        
//    }
//
//    virtual int GetPay() const override
//    {
//        return salary;
//    }
//
//    virtual void ShowSalaryInfo() const override
//    {
//        ShowYourName();
//        cout << "salary: " << GetPay() << "\n\n";
//    }
//};
//
//class TemporaryWorker : public Employee
//{
//private:
//    int workTime;
//    int payPerHour;
//
//public:
//    TemporaryWorker(const char* name, int pay) : Employee(name), workTime(0), payPerHour(pay)
//    {
//
//    }
//
//    void AddWorkTime(int time)
//    {
//        workTime += time;
//    }
//
//    virtual int GetPay() const override
//    {
//        return workTime * payPerHour;
//    }
//
//    virtual void ShowSalaryInfo() const override
//    {
//        ShowYourName();
//        cout << "salary: " << GetPay() << "\n\n";
//    }
//};
//
//class SalesWorker : public PermanentWorker
//{
//private:
//    int salesResult;
//    double bonusRatio;
//
//public:
//    SalesWorker(const char* name, int money, double ratio)
//        : PermanentWorker(name, money), salesResult(0), bonusRatio(ratio)
//    {
//
//    }
//
//    virtual void AddSalesResult(int value)
//    {
//        salesResult += value;
//    }
//
//    virtual int GetPay() const override
//    {
//        return PermanentWorker::GetPay() + static_cast<int>(salesResult * bonusRatio);
//    }
//
//    virtual void ShowSalaryInfo() const override
//    {
//        ShowYourName();
//        cout << "salary: " << GetPay() << "\n\n";
//    }
//};
//
//class ForeignSalesWorker : public SalesWorker
//{
//private:
//    int risk;
//
//public:
//    ForeignSalesWorker(const char* name, int money, double ratio, int r)
//        : SalesWorker(name, money, ratio), risk(r)
//    {
//        
//    }
//
//    virtual void AddSalesResult(int value) override
//    {
//        SalesWorker::AddSalesResult(value);
//    }
//
//    virtual int GetPay() const override
//    {
//        return SalesWorker::GetPay();
//    }
//
//    virtual void ShowSalaryInfo() const override
//    {
//        ShowYourName();
//        cout << "salary: " << GetPay() << "\n";
//        
//        if (risk == RISK::RISK_A)
//        {
//            cout << "risk pay: " << GetPay() * 0.3 << "\n";
//            cout << "sum: " << GetPay() + GetPay() * 0.3 << "\n\n";
//        }
//        else if (risk == RISK::RISK_B)
//        {
//            cout << "risk pay: " << GetPay() * 0.2 << "\n";
//            cout << "sum: " << GetPay() + GetPay() * 0.2 << "\n\n";
//        }
//        else if (risk == RISK::RISK_C)
//        {
//            cout << "risk pay: " << GetPay() * 0.1 << "\n";
//            cout << "sum: " << GetPay() + GetPay() * 0.1 << "\n\n";
//        }
//    }
//};
//
//class EmployeeHandler
//{
//private:
//    Employee* empList[50];
//    int empNum;
//
//public:
//    EmployeeHandler() : empNum(0)
//    {
//        
//    }
//
//    void AddEmployee(Employee* emp)
//    {
//        empList[empNum++] = emp;
//    }
//
//    void ShowAllSalaryInfo() const
//    {
//        for (int i = 0; i < empNum; i++)
//        {
//            empList[i]->ShowSalaryInfo();
//        }
//    }
//
//    void ShowTotalSalary() const
//    {
//        int sum = 0;
//        for (int i = 0; i < empNum; i++)
//        {
//            sum += empList[i]->GetPay();
//        }
//
//        cout << "salary sum: " << sum << "\n";
//    }
//
//    ~EmployeeHandler()
//    {
//        for (int i = 0; i < empNum; i++)
//        {
//            delete empList[i];
//        }
//    }
//};
//
//int main()
//{
//    EmployeeHandler handler;
//
//    ForeignSalesWorker* fseller1 = new ForeignSalesWorker("Hong", 1000, 0.1, RISK::RISK_A);
//    fseller1->AddSalesResult(7000);
//    handler.AddEmployee(fseller1);
//
//    ForeignSalesWorker* fseller2 = new ForeignSalesWorker("Yoon", 1000, 0.1, RISK::RISK_B);
//    fseller2->AddSalesResult(7000);
//    handler.AddEmployee(fseller2);
//
//    ForeignSalesWorker* fseller3 = new ForeignSalesWorker("Lee", 1000, 0.1, RISK::RISK_C);
//    fseller3->AddSalesResult(7000);
//    handler.AddEmployee(fseller3);
//
//    handler.ShowAllSalaryInfo();
//
//    return 0;
//}