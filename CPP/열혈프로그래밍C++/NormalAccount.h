#pragma once
#include "Account.h"
#include "String.h"

class NormalAccount : public Account
{
private:
    int ratio;

public:
    NormalAccount(int ac, String n, int m, int r) : Account(ac, n, m), ratio(r)
    {

    }

    virtual void InputMoney(int id, int m) override
    {
        Account::InputMoney(id, m);
    }

    virtual double GetRatio() override
    {
        return ratio * 0.01;
    }
};