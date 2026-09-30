#pragma once
#include <Windows.h>

#define MSG_MAX_LEN 65536
#define BUFSIZE 50000

struct SESSION
{
    SOCKET sock;
    unsigned int id;
    char recvQ[BUFSIZE];
    unsigned int recvByte;
    char sendQ[BUFSIZE];
    unsigned int sendByte;
};

#pragma pack(push, 1)
struct HEADER
{
    unsigned char type;
    unsigned short packetSize;
};
#pragma pack(pop)