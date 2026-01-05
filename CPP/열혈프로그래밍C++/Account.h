#pragma once
#include "String.h"

class Account
{
private:
    int acc;
    String name;
    int money;

public:
    Account(int ac, String n, int m);
    Account(const Account& account);
    ~Account();

    Account& operator=(const Account& account);
    virtual void InputMoney(int id, int m);
    virtual double GetRatio() = 0;
    virtual void OutputMoney(int id, int m);
    virtual void ShowAllAccount() const;
};

