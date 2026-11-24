#include "LanServer.h"
#include "CpuUsage.h"
#include <iostream>
#include <process.h>
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
        SerializationBuffer* packet = new SerializationBuffer;
        packet->clear();

        short header = sizeof(int64_t);
        int64_t data = 0x7fffffffffffffff;

        *packet << header;
        *packet << data;

        sendPacket(sessionID, packet);
    }

    void OnRelease(uint64_t sessionID) override
    {
        
    }

    void OnRecv(uint64_t sessionID, SerializationBuffer* packet) override
    {
        SerializationBuffer* echo = new SerializationBuffer;
        echo->clear();
        echo->putData(packet->getBufferPtr(), packet->getDataSize());
        sendPacket(sessionID, echo);
    }

private:
};

Contents server;

uint32_t WINAPI cpuTimeThread(LPVOID param);

int wmain()
{
    timeBeginPeriod(1);

    SYSTEM_INFO si;
    GetSystemInfo(&si);

    HANDLE hThread;
    hThread = (HANDLE)_beginthreadex(NULL, 0, cpuTimeThread, NULL, 0, NULL);

    if (!server.Start(SERVERPORT, 6, true, si.dwNumberOfProcessors))
    {
        return 1;
    }

    server.Update();
    server.Stop();
    CloseHandle(hThread);

    return 0;
}

uint32_t cpuTimeThread(LPVOID param)
{
    CpuUsage cpuTime(GetCurrentProcess());

    while (1)
    { 
        cpuTime.updateCpuTime();
        wprintf(L"Processor:%f / Process:%f \n", cpuTime.processorTotal(), cpuTime.processTotal());
        wprintf(L"ProcessorKernel:%f / ProcessKernel:%f \n", cpuTime.processorKernel(), cpuTime.processKernel());
        wprintf(L"ProcessorUser:%f / ProcessUser:%f \n", cpuTime.processorUser(), cpuTime.processUser());
        Sleep(1000); 
    }

    return 0;
}
