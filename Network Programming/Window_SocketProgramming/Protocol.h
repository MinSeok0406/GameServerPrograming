#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <windowsx.h>
#include "SerializationBuffer.h"
#include "RingBuffer.h"

#define ﻿dfNETWORK_PACKET_CODE      0x89
#define dfRANGE_MOVE_TOP	       50
#define dfRANGE_MOVE_LEFT	       10
#define dfRANGE_MOVE_RIGHT	       630
#define dfRANGE_MOVE_BOTTOM	       470
#define dfERROR_RANGE		       50
#define BUFSIZE                    5000

struct st_HEADER
{
    unsigned char	_byCode;			// 패킷코드 0x89 고정.
    unsigned char	_bySize;			// 패킷 사이즈.
    unsigned char	_byType;			// 패킷타입.
};

#pragma pack(1)
struct st_SESSION
{
    SOCKET          _socket;
    unsigned int    _dwSessionID;
    wchar_t         _ip[INET_ADDRSTRLEN];
    unsigned short  _port;
    RingBuffer      _recvQ { BUFSIZE };
    RingBuffer      _sendQ { BUFSIZE };

    unsigned int    _dwAction;
    unsigned char   _byDirection;

    short           _shX;
    short           _shY;

    char            _chHP;
};

#define	dfPACKET_SC_CREATE_MY_CHARACTER			0
struct st_SC_CREATE_MY_CHARACTER
{
    unsigned int    _id;
    unsigned char   _direction;
    short           _x;
    short           _y;
    char            _hp;
};

#define	dfPACKET_SC_CREATE_OTHER_CHARACTER		1
struct st_SC_CREATE_OTHER_CHARACTER
{
    unsigned int    _id;
    unsigned char   _direction;
    short           _x;
    short           _y;
    char            _hp;
};

#define	dfPACKET_SC_DELETE_CHARACTER			2
struct st_SC_DELETE_CHARACTER
{
    unsigned int    _id;
};

#define	dfPACKET_CS_MOVE_START					10
#define dfPACKET_MOVE_DIR_LL					0
#define dfPACKET_MOVE_DIR_LU					1
#define dfPACKET_MOVE_DIR_UU					2
#define dfPACKET_MOVE_DIR_RU					3
#define dfPACKET_MOVE_DIR_RR					4
#define dfPACKET_MOVE_DIR_RD					5
#define dfPACKET_MOVE_DIR_DD					6
#define dfPACKET_MOVE_DIR_LD					7
struct st_CS_MOVE_START
{
    unsigned char   _direction;
    short           _x;
    short           _y;
};

#define	dfPACKET_SC_MOVE_START					11
struct st_SC_MOVE_START
{
    unsigned int    _id;
    unsigned char   _direction;
    short           _x;
    short           _y;
};

#define	dfPACKET_CS_MOVE_STOP					12
struct st_CS_MOVE_STOP
{
    unsigned char   _direction;
    short           _x;
    short           _y;
};

#define	dfPACKET_SC_MOVE_STOP					13
struct st_SC_MOVE_STOP
{
    unsigned int    _id;
    unsigned char   _direction;
    short           _x;
    short           _y;
};

#define	dfPACKET_CS_ATTACK1						20
struct st_CS_ATTACK1
{
    unsigned char   _direction;
    short           _x;
    short           _y;
};

#define	dfPACKET_SC_ATTACK1						21
struct st_SC_ATTACK1
{
    unsigned int    _id;
    unsigned char   _direction;
    short           _x;
    short           _y;
};

#define	dfPACKET_CS_ATTACK2						22
struct st_CS_ATTACK2
{
    unsigned char   _direction;
    short           _x;
    short           _y;
};

#define	dfPACKET_SC_ATTACK2						23
struct st_SC_ATTACK2
{
    unsigned int    _id;
    unsigned char   _direction;
    short           _x;
    short           _y;
};

#define	dfPACKET_CS_ATTACK3						24
struct st_CS_ATTACK3
{
    unsigned char   _direction;
    short           _x;
    short           _y;
};

#define	dfPACKET_SC_ATTACK3						25
struct st_SC_ATTACK3
{
    unsigned int    _id;
    unsigned char   _direction;
    short           _x;
    short           _y;
};

#define	dfPACKET_SC_DAMAGE						30
struct st_SC_DAMAGE
{
    unsigned int    _attackID;  // ( 공격자 ID )
    unsigned int    _damageID;  // ( 피해자 ID )
    char            _damageHP;  // ( 피해자 남은 HP )
};

#define	dfPACKET_CS_SYNC						250
struct st_CS_SYNC
{
    short           _x;
    short           _y;
};

#define	dfPACKET_SC_SYNC						251
struct st_SC_SYNC
{
    unsigned int    _id;
    short           _x;
    short           _y;
};