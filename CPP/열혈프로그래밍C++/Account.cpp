#include "Account.h"
#include "Main.h"

Account::Account(int ac, String n, int m)
{
    acc = ac;
    money = m;
    name = n;
}

Account::Account(const Account& account)
{
    acc = account.acc;
    money = account.money;
    name = account.name;
}

Account::~Account()
{
    
}

Account& Account::operator=(const Account& account) 
{
    name = account.name;
    acc = account.acc;
    money = account.money;
    return *this;
}

void Account::InputMoney(int id, int m)
{
    if (this->acc == id)
    {
        this->money += static_cast<int>(m + this->money * GetRatio());
        cout << "입금완료" << "\n\n";
    }
}

void Account::OutputMoney(int id, int m)
{
    if (this->acc == id)
    {
        this->money -= m;
        cout << "출금완료" << "\n\n";
    }
}

void Account::ShowAllAccount() const
{
    cout << "\n\n";

    cout << "계좌ID: " << this->acc << "\n";
    cout << "이 름: " << this->name << "\n";
    cout << "잔 액: " << this->money << "\n";
    cout << "\n";
}

