#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <windowsx.h>
#include "RingBuffer.h"
#include "SerializationBuffer.h"

#define SENDBUFSIZE     50000
#define RECVBUFSIZE     5000

struct HEADER
{
    unsigned char _packetsize;
    unsigned char _type;
};

#pragma pack(1)
struct USER
{
    unsigned int    _id;
    char            _name[20];
    RingBuffer      _sendQ { SENDBUFSIZE };
    RingBuffer      _recvQ { RECVBUFSIZE };
};

#define PACKET_SC_CREATE_USER       0
struct SC_CREATE_USER
{
    unsigned int    _id;
    char            _name[20];
};

#define PACKET_SC_OTHER_USER        1
struct SC_OTHER_USER
{
    unsigned int    _id;
    char            _name[20];
};

#define PACKET_CS_MSG               2
struct CS_MSG
{
    unsigned char   _len;
    char            _msg[200];
};

#define PACKET_SC_MSG               3
struct SC_MSG
{
    unsigned char   _len;
    char            _msg[200];
};