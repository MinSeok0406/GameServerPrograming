#pragma once
#include "Account.h"
#include "BoundCheckArray.h"

class AccountHandler
{
private:
    BoundCheckArray<Account*> acc;
    int cnt;

public:
    AccountHandler();
    ~AccountHandler();
    void AddAccount();
    void InputMoney();
    void OutputMoney();
    void ShowAccount();
};

