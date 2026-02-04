export module Employee;

import <iostream>;
import <format>;
import <string>;
using namespace std;

namespace Records
{
    const int DefaultStartingSalary { 30'000 };
    export const int DefaultRaiseAndDemeritAmount { 1'000 };

    export class Employee
    {
    public:
        Employee(const string& firstName, const string& lastName);

        void promote(int raiseAmount = DefaultStartingSalary);
        void demote(int demeritAmount = DefaultStartingSalary);
        void hire();
        void fire();
        void display() const;

        void setFirstName(const string& firstName);
        const string& getFirstName() const;

        void setLastName(const string& lastName);
        const string& getLastName() const;

        void setEmployeeNumber(int employeeNumber);
        int getEmployeeNumber() const;

        void setSalary(int newSalary);
        int getSalary() const;

        bool isHired() const;

    private:
        string m_firstName;
        string m_lastName;
        int m_employeeNumber { -1 };
        int m_salary { DefaultStartingSalary };
        bool m_hired { false };
    };
}

