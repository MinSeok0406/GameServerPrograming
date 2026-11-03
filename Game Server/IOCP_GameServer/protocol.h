#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include "RingBuffer.h"

enum class TYPE
{
    SEND,
    RECV
};

struct SendOverlapped
{
    WSAOVERLAPPED overlapped;
    uint32_t sessionID;
};

struct RecvOverlapped
{
    WSAOVERLAPPED overlapped;
    uint32_t sessionID;
};

struct Session
{
    SOCKET socket;
    wchar_t ip[INET_ADDRSTRLEN];
    uint16_t port;
    RingBuffer sendQ { 50000 };
    RingBuffer recvQ { 5000 };
    SendOverlapped* sendOL;
    RecvOverlapped* recvOL;
};