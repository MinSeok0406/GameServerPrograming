#pragma once
#include "Account.h"
#include "String.h"

enum GRADE
{
    A = 7,
    B = 4,
    C = 2
};

class HighCreditAccount : public Account
{
private:
    int ratio;
    int grade;

public:
    HighCreditAccount(int ac, String n, int m, int r, int gr) : Account(ac, n, m), ratio(r), grade(gr)
    {

    }

    virtual void InputMoney(int id, int m) override
    {
        Account::InputMoney(id, m);
    }

    virtual double GetRatio() override
    {
        if (grade == 1)
        {
            ratio += A;
            return (ratio * 0.01);
        }
        else if (grade == 2)
        {
            ratio += B;
            return (ratio * 0.01);
        }
        else if (grade == 3)
        {
            ratio += C;
            return (ratio * 0.01);
        }
    }
};