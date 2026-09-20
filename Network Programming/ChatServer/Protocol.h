#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <windowsx.h>
#include "RingBuffer.h"
#include "SerializationBuffer.h"
#include "ObjectFreeList.h"

#define RECVBUFSIZE     5000

struct PendingPacket
{
    SerializationBuffer* buf;
    uint32_t refCount;
};

struct SendItem
{
    PendingPacket* packet;
    int sentOffset;
    SendItem* next;
};

#pragma pack(1)
struct HEADER
{
    unsigned short _packetsize;
    unsigned char _type;
};

#pragma pack(1)
struct USER
{
    SOCKET          _sock;
    unsigned int    _id;
    char            _name[20];
    unsigned int    _namesize;
    wchar_t         _ip[INET_ADDRSTRLEN];
    unsigned short  _port;
    SendItem*       _sendHead = nullptr;
    SendItem*       _sendTail = nullptr;
    uint32_t        _sendQueueCount = 0;
    RingBuffer      _recvQ { RECVBUFSIZE };
    bool            _disconnected;
};

#define PACKET_SC_CREATE_USER       0
struct SC_CREATE_USER
{
    unsigned int    _id;
    unsigned int    _namesize;
    char            _name[20];
};

#define PACKET_SC_OTHER_USER        1
struct SC_OTHER_USER
{
    unsigned int    _id;
    unsigned int    _namesize;
    char            _name[20];
};

#define PACKET_CS_MSG               2
struct CS_MSG
{
    unsigned short  _len;
    unsigned int    _namesize;
    char            _name[20];
    char            _msg[500];
};

#define PACKET_SC_MSG               3
struct SC_MSG
{
    unsigned short  _len;
    unsigned int    _namesize;
    char            _name[20];
    char            _msg[500];
};