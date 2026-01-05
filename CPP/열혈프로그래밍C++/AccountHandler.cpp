#include "AccountHandler.h"
#include "NormalAccount.h"
#include "HighCreditAccount.h"
#include "Main.h"

AccountHandler::AccountHandler() : cnt(0)
{
    
}

AccountHandler::~AccountHandler()
{
    for (int i = 0; i < cnt; i++)
    {
        delete acc[i];
    }
}

void AccountHandler::AddAccount()
{
    int id, m, cho, ratio, grade;
    String nm;
    cout << "\n\n";
    cout << "[계좌종류선택]" << "\n";
    cout << "1.보통예금계좌 2.신용신뢰계좌\n";
    cout << "선택: ";
    cin >> cho;
    if (cho == 1)
    {
        cout << "[보통계좌개설]" << "\n";
        cout << "계좌ID: ";
        cin >> id;
        cout << "이 름: ";
        cin >> nm;
        cout << "입금액: ";
        cin >> m;
        cout << "이자율: ";
        cin >> ratio;
        acc[cnt++] = new NormalAccount(id, nm, m, ratio);
    }
    else if (cho == 2)
    {
        cout << "[신용신뢰계좌 개설]" << "\n";
        cout << "계좌ID: ";
        cin >> id;
        cout << "이 름: ";
        cin >> nm;
        cout << "입금액: ";
        cin >> m;
        cout << "이자율: ";
        cin >> ratio;
        cout << "신용등급(1toA, 2toB, 3toC): ";
        cin >> grade;
        acc[cnt++] = new HighCreditAccount(id, nm, m, ratio, grade);
    }
}

void AccountHandler::InputMoney()
{
    int id, money;
    cout << "\n\n";
    cout << "[입    금]" << "\n";
    cout << "계좌ID: ";
    cin >> id;
    cout << "입금액: ";
    cin >> money;

    for (int i = 0; i < cnt; i++)
    {
        acc[i]->InputMoney(id, money);
    }
}

void AccountHandler::OutputMoney()
{
    int id, money;
    cout << "\n\n";
    cout << "[출    금]" << "\n";
    cout << "계좌ID: ";
    cin >> id;
    cout << "출금액: ";
    cin >> money;

    for (int i = 0; i < cnt; i++)
    {
        acc[i]->OutputMoney(id, money);
    }
}

void AccountHandler::ShowAccount()
{
    for (int i = 0; i < cnt; i++)
    {
        acc[i]->ShowAllAccount();
    }
}