#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include "RingBuffer.h"
#include "SerializationBuffer.h"

constexpr int ce_SendArraySize = (1 << 12);
constexpr int ce_SendArrayMask = ce_SendArraySize - 1;
constexpr int ce_MaxWsaBuf = 500;

enum class TYPE
{
    SEND,
    RECV
};

struct SendOverlapped
{
    WSAOVERLAPPED overlapped;
    uint32_t overlappedType;
};

struct RecvOverlapped
{
    WSAOVERLAPPED overlapped;
    uint32_t overlappedType;
};

struct Session
{
    uint64_t sessionID;
    SOCKET socket;
    wchar_t ip[INET_ADDRSTRLEN];
    uint16_t port;
    RingBuffer recvQ { 5000 };

    // 링 버퍼를 대체하는 배열
    SerializationBuffer* sendArray[ce_SendArraySize];
    long sendRear;
    long sendFront;
    long sendArrayCount;

    // WSASend에 걸려있는 패킷
    SerializationBuffer* sendingPackets[ce_MaxWsaBuf];
    long sendingCount;

    SendOverlapped* sendOL;
    RecvOverlapped* recvOL;
    long count;
    long sendflag;
};