#pragma once
#include <map>
#include <array>
#include <stack>
#include "protocol.h"
#include "SerializationBuffer.h"

#define CLIENT 6

constexpr uint64_t MAX_SESSION = 64 * 1024;
constexpr uint64_t INDEX_MASK = MAX_SESSION - 1;

class LanServer
{
public:
    LanServer() : listen_sock(NULL), hcp(NULL)
    {
        // 세션 배열 초기화
        sessions.fill(nullptr);
        for (int i = MAX_SESSION - 1; i >= 0; --i)
        {
            sessionIndex.push(static_cast<uint16_t>(i));
        }

        for (auto i = 0; i < CLIENT; ++i)
        {
            hThreads[i] = NULL;
        }
    }

    virtual ~LanServer()
    {
    }

    LanServer(const LanServer&) = delete;
    LanServer& operator=(const LanServer&) = delete;
    
    bool Start(uint16_t port, unsigned long workerThread, bool isNagle, unsigned long NumberOfConcurrentThreads);
    void Stop();
    void Update();
    //int GetSessionCount() { return (int)map_session.size(); }

    bool disconnect(uint64_t sessionID);
    bool sendPacket(uint64_t sessionID, SerializationBuffer* packet);

    // 컨텐츠에서 재정의하는 함수
    virtual bool OnConnectionRequest(const wchar_t* ip, uint16_t port) = 0;
    virtual void OnAccept(uint64_t sessionID, const wchar_t* ip, uint16_t port) = 0;
    virtual void OnRelease(uint64_t sessionID) = 0;
    virtual void OnRecv(uint64_t sessionID, SerializationBuffer* packet) = 0;

private:
    static uint32_t WINAPI IOCPThreadProc(LPVOID arg);
    static uint32_t WINAPI acceptThreadProc(LPVOID param);
    void IOCPThread();
    void acceptThread();

    bool sendPost(Session* session);
    bool recvPost(Session* session);

    bool releaseSession(Session* session);

    SOCKET listen_sock;
    //std::map<uint64_t, Session*> map_session;

    // 동기화 필요 -> 락을 사용하든 락프리를 사용하든 해결해야 함
    std::array<Session*, MAX_SESSION> sessions;
    std::stack<uint16_t> sessionIndex;

    uint64_t idCounter = 0;
    long sessionCount = 0;

    HANDLE hcp;
    HANDLE hThreads[CLIENT];
};