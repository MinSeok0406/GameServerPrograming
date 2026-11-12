#include <iostream>
#include <process.h>
#include <chrono>
#include <conio.h>
#include <map>
#include "LanServer.h"
using namespace std;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

#define SERVERPORT 6000

class Contents : public LanServer
{
public:
    Contents() = default;
    ~Contents() = default;

    // LanServer을(를) 통해 상속됨
    bool OnConnectionRequest(const wchar_t* ip, uint16_t port) override
    {

        return true;
    }

    void OnAccept(uint64_t sessionID, const wchar_t* ip, uint16_t port) override
    {

    }

    void OnRelease(uint64_t sessionID) override
    {

    }

    void OnRecv(uint64_t sessionID, SerializationBuffer* packet) override
    {
        sendPacket(sessionID, packet);
    }

private:
};

Contents server;

int wmain()
{
    timeBeginPeriod(1);

    SYSTEM_INFO si;
    GetSystemInfo(&si);

    if (!server.Start(SERVERPORT, 6, true, si.dwNumberOfProcessors))
    {
        return 1;
    }

    server.Update();
    server.Stop();

    return 0;
}

