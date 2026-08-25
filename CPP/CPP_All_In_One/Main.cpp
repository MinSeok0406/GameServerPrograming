#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <tchar.h>
#include <process.h>
#include <Windows.h>
#include <functional>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <stack>
#include <queue>
#include <list>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")

class CompanyA
{
public:
    void sendClearText(const string& msg)
    {

    }

    void sendEncrypted(const string& msg)
    {

    }
};

class CompanyB
{
public:
    void sendClearText(const string& msg)
    {

    }

    void sendEncrypted(const string& msg)
    {

    }
};

class MsgInfo
{

};

template<typename Company>
class MsgSender
{
public:
    void sendClear(const MsgInfo& info)
    {
        string msg;

        Company c;
        c.sendClearText(msg);
    }

    void sendSecret(const MsgInfo& info)
    {
        string msg;

        Company c;
        c.sendEncrypted(msg);
    }
};

template<typename Company>
class LoggingMsgSender : public MsgSender<Company>
{
public:
    using MsgSender<Company>::sendClear;

    void sendClearMsg(const MsgInfo& info)
    {
        sendClear(info);
    }
};

int wmain(int argc, WCHAR* argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    timeBeginPeriod(1);

    

    return 0;
}
