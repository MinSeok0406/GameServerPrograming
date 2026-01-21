#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <mutex>
#include <cmath>
#include <format>
#include <list>
#include <optional>
#include <set>
using namespace std;

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

class CompanyZ
{
public:
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
    void SendClear(const MsgInfo& info)
    {
        string msg;

        Company c;
        c.sendClearText(msg);
    }

    void SendSecret(const MsgInfo& info)
    {
        string msg;

        Company c;
        c.sendEncrypted(msg);
    }
};

template<>
class MsgSender<CompanyZ>
{
public:
    void SendSecret(const MsgInfo& info)
    {
        string msg;

    }
};

// 1锅掳 规过
template<typename Company>
class LoggingMsgSender : public MsgSender<Company>
{
public:
    void SendClearMsg(const MsgInfo& info)
    {
        this->SendClear(info);
    }

    void SendSecretMsg(const MsgInfo& info)
    {
        this->SendSecret(info);
    }
};

// 2锅掳 规过
template<typename Company>
class LoggingMsgSender2 : public MsgSender<Company>
{
public:
    using MsgSender<Company>::SendClear;
    using MsgSender<Company>::SendSecret;

    void SendClearMsg(const MsgInfo& info)
    {
        SendClear(info);
    }

    void SendSecretMsg(const MsgInfo& info)
    {
        SendSecret(info);
    }
};

// 3锅掳 规过
template<typename Company>
class LoggingMsgSender3 : public MsgSender<Company>
{
public:
    void SendClearMsg(const MsgInfo& info)
    {
        MsgSender<Company>::SendClear(info);
    }

    void SendSecretMsg(const MsgInfo& info)
    {
        MsgSender<Company>::SendSecret(info);
    }
};

int main()
{
    LoggingMsgSender<CompanyZ> cz;

    MsgInfo msg;
    
    cz.SendClearMsg(msg);

    return 0;
}