#pragma once
#include <map>
#include "protocol.h"
#include "SerializationBuffer.h"

#define CLIENT 6

class LanServer
{
public:
    LanServer() : event_accept(NULL), listen_sock(NULL), hcp(NULL)
    {
        InitializeSRWLock(&srw_session);

        for (auto i = 0; i < CLIENT; ++i)
        {
            hThreads[i] = NULL;
        }
    }

    ~LanServer()
    {
        Stop();
    }

    LanServer(const LanServer&) = delete;
    LanServer& operator=(const LanServer&) = delete;
    
    bool Start(uint16_t port, unsigned long workerThread, bool isNagle, unsigned long NumberOfConcurrentThreads);
    void Stop();
    void Update();
    int GetSessionCount() { return (int)map_session.size(); }

    bool disconnect(uint64_t sessionID);
    bool sendPacket(uint64_t sessionID, SerializationBuffer* packet);

    // 컨텐츠에서 재정의하는 함수
    virtual bool OnConnectionRequest(const wchar_t* ip, uint16_t port) = 0;
    virtual void OnAccept(uint64_t sessionID, const wchar_t* ip, uint16_t port) = 0;
    virtual void OnRelease(uint64_t sessionID) = 0;
    virtual void OnRecv(uint64_t sessionID, SerializationBuffer* packet) = 0;

private:
    static uint32_t WINAPI IOCPworkerThread(LPVOID arg);
    static uint32_t WINAPI acceptThreadProc(LPVOID param);
    void acceptThread();

    bool sendPost(Session* session);
    bool recvPost(Session* session);

    SOCKET listen_sock;
    std::map<uint64_t, Session*> map_session;

    SRWLOCK srw_session;
    HANDLE hcp;
    HANDLE event_accept;
    HANDLE hThreads[CLIENT];
};