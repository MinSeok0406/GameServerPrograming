#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <windowsx.h>
#include "RingBuffer.h"

#define SENDBUFSIZE     50000
#define RECVBUFSIZE     5000

struct st_HEADER
{
    unsigned char _packetsize;
    unsigned char _type;
};

#pragma pack(1)
struct st_USER
{
    SOCKET          _sock;
    unsigned int    _id;
    wchar_t         _ip[INET_ADDRSTRLEN];
    unsigned short  _port;
    RingBuffer      _sendQ { SENDBUFSIZE };
    RingBuffer      _recvQ { RECVBUFSIZE };
};

#define dfPACKET_SC_CREATE_USER     0
struct st_SC_CREATE_USER
{
    unsigned int _id;
    char name[20];
};

#define dfPACKET_SC_OTHER_USER      1
struct st_SC_OTHER_USER
{
    unsigned int _id;
    char name[20];
};