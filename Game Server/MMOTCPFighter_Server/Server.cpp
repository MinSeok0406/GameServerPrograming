#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <time.h>
#include <list>
#include <algorithm>
#include "Protocol.h"
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

#define SERVERPORT 5000

list<st_SESSION> g_sessionList;
SOCKET g_listenSocket;
SOCKET g_clientSocket;
SOCKADDR_IN g_clientaddr;
bool g_shutdown = false;

static int g_id = 0;

// 세션 생성 및 삭제 함수
bool fcreateSession();
bool fdisconnect(st_SESSION* session);

// 네트워크 함수
bool netProc_Accept();
bool netProc_Recv(st_SESSION* session);
bool netProc_Send(st_SESSION* session);
bool sendPacket_Unicast(st_SESSION* session, SerializationBuffer* packet);
bool sendPacket_Broadcast(st_SESSION* session, SerializationBuffer* packet);
bool sendPacket_Around(st_SESSION* session, SerializationBuffer* packet);
bool packetProc(st_SESSION* session, unsigned char packetType, SerializationBuffer* packet);

// 컨텐츠 함수
bool netPacketProc_MoveStart(st_SESSION* session, SerializationBuffer* packet);
bool netPacketProc_MoveStop(st_SESSION* session, SerializationBuffer* packet);
bool netPacketProc_Attack1(st_SESSION* session, SerializationBuffer* packet);
bool netPacketProc_Attack2(st_SESSION* session, SerializationBuffer* packet);
bool netPacketProc_Attack3(st_SESSION* session, SerializationBuffer* packet);

// 전체적인 게임 함수
bool netIOProcess();
bool Update();

// 네트워크 프로토콜 함수
bool npfCreateCharacter(SerializationBuffer* packet, unsigned int id,
	unsigned char direction, short x, short y, char hp);
bool npfCreateOtherCharacter(SerializationBuffer* packet, unsigned int id,
	unsigned char direction, short x, short y, char hp);
bool npfDeleteCharacter(SerializationBuffer* packet, unsigned int id);
bool npfMoveStart(SerializationBuffer* packet, unsigned int id, unsigned char direction,
	short x, short y);
bool npfMoveStop(SerializationBuffer* packet, unsigned int id, unsigned char direction,
	short x, short y);
bool npfAttack1(SerializationBuffer* packet, unsigned int id, unsigned char direction,
	short x, short y);
bool npfAttack2(SerializationBuffer* packet, unsigned int id, unsigned char direction,
	short x, short y);
bool npfAttack3(SerializationBuffer* packet, unsigned int id, unsigned char direction,
	short x, short y);
bool npfDamage(SerializationBuffer* packet, unsigned int attackID, unsigned int damageID,
	char damageHP);
bool npfSync(SerializationBuffer* packet, unsigned int id, short x, short y);

int wmain(int argc, WCHAR* argv[])
{
	timeBeginPeriod(1);
	srand(unsigned int(time(nullptr)));

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 0;
	}

	g_listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (g_listenSocket == INVALID_SOCKET)
	{
		printf("socket : %d\n", WSAGetLastError());
		return 0;
	}

	SOCKADDR_IN serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	
	auto bindRet = bind(g_listenSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (bindRet == SOCKET_ERROR)
	{
		printf("bind : %d\n", WSAGetLastError());
		return 0;
	}

	u_long mode = 1;
	auto ioctRet = ioctlsocket(g_listenSocket, FIONBIO, &mode);
	if (ioctRet == SOCKET_ERROR)
	{
		printf("ioct : %d\n", WSAGetLastError());
		return 0;
	}

	auto listenRet = listen(g_listenSocket, SOMAXCONN);
	if (listenRet == SOCKET_ERROR)
	{
		printf("listen : %d\n", WSAGetLastError());
		return 0;
	}

	unsigned int tick = timeGetTime();
	while (!g_shutdown)
	{
		netIOProcess();
		Update();

		int useTime = (int)(timeGetTime() - tick);
		if (useTime < 20)
		{
			Sleep(20 - useTime);
		}
		else if (useTime > 100)
		{
			tick = timeGetTime();
		}
		tick += 20;
	}

	closesocket(g_listenSocket);
	WSACleanup();
	return 1;
}

bool fcreateSession()
{
	g_sessionList.emplace_back();
	st_SESSION& createSession = g_sessionList.back();
	createSession._socket = g_clientSocket;
	createSession._dwSessionID = g_id++;
	InetNtop(AF_INET, &g_clientaddr.sin_addr, createSession._ip, sizeof(createSession._ip));
	createSession._port = ntohs(g_clientaddr.sin_port);
	createSession._dwAction = 8;
	createSession._byDirection = dfPACKET_MOVE_DIR_RR;
	createSession._shX = (rand() % 600) + 15;
	createSession._shY = (rand() % 410) + 55;
	createSession._chHP = 100;

	SerializationBuffer packet;

	npfCreateCharacter(&packet, createSession._dwSessionID, createSession._byDirection,
		createSession._shX, createSession._shY, createSession._chHP);
	sendPacket_Unicast(&createSession, &packet);
	packet.clear();

	npfCreateOtherCharacter(&packet, createSession._dwSessionID, createSession._byDirection,
		createSession._shX, createSession._shY, createSession._chHP);
	sendPacket_Broadcast(&createSession, &packet);
	packet.clear();

	for (auto& session : g_sessionList)
	{
		if (session._dwSessionID != createSession._dwSessionID)
		{
			npfCreateOtherCharacter(&packet, session._dwSessionID,
				session._byDirection, session._shX, session._shY, session._chHP);
			sendPacket_Unicast(&createSession, &packet);
			packet.clear();
		}
	}

	return true;
}

bool fdisconnect(st_SESSION* session)
{
	return true;
}

bool netProc_Accept()
{
	int addrlen = sizeof(g_clientaddr);
	g_clientSocket = accept(g_listenSocket, (SOCKADDR*)&g_clientaddr, &addrlen);
	if (g_clientSocket == INVALID_SOCKET)
	{
		printf("accept : %d\n", WSAGetLastError());
		return false;
	}

	fcreateSession();

	return true;
}

bool netProc_Recv(st_SESSION* session)
{
	if (session->_recvQ.GetFreeSize() < sizeof(st_HEADER) + 12)
	{
		return false;
	}

	int recvRet = recv(session->_socket, session->_recvQ.GetRearBufferPtr(),
		session->_recvQ.DirectEnqueueSize(), 0);
	if (recvRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			printf("recv : %d\n", WSAGetLastError());
			session->_chHP = 0;
		}

		return true;
	}
	else if (recvRet == 0)
	{
		session->_chHP = 0;
		return true;
	}

	session->_recvQ.MoveRear(recvRet);

	while (true)
	{
		if (session->_recvQ.GetUseSize() <= sizeof(st_HEADER))
		{
			return false;
		}

		char buf[sizeof(st_HEADER)];
		int peekRet = session->_recvQ.Peek(buf, sizeof(st_HEADER));
		if (peekRet != sizeof(st_HEADER))
		{
			__debugbreak();
		}

		st_HEADER* header = (st_HEADER*)buf;
		if (header->_byCode != ﻿dfNETWORK_PACKET_CODE)
		{
			session->_recvQ.MoveFront(1);
			continue;
		}

		session->_recvQ.MoveFront(sizeof(st_HEADER));

		SerializationBuffer packet;
		peekRet = session->_recvQ.Peek(packet.getBufferPtr(), header->_bySize);
		if (peekRet != header->_bySize)
		{
			__debugbreak();
		}
		session->_recvQ.MoveFront(peekRet);
		packet.moveReadPos(peekRet);

		packetProc(session, header->_byType, &packet);
	}

	return true;
}

bool netProc_Send(st_SESSION* session)
{
	while (true)
	{
		if (session->_sendQ.GetUseSize() < sizeof(st_HEADER))
		{
			return true;
		}

		int sendRet = send(session->_socket, session->_sendQ.GetFrontBufferPtr(),
			session->_sendQ.DirectDequeueSize(), 0);
		if (sendRet == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				printf("send : %d\n", WSAGetLastError());
				session->_chHP = 0;
			}

			return true;
		}

		session->_sendQ.MoveFront(sendRet);
	}

	return true;
}

bool sendPacket_Unicast(st_SESSION* session, SerializationBuffer* packet)
{
	if (session->_sendQ.GetFreeSize() < packet->getDataSize())
	{
		return false;
	}

	int size = packet->getDataSize();
	int enqueueRet = session->_sendQ.Enqueue(packet->getBufferPtr(), size);
	if (enqueueRet != size)
	{
		session->_sendQ.Dequeue(packet->getBufferPtr(), enqueueRet);
		return false;
	}

	return true;
}

bool sendPacket_Broadcast(st_SESSION* session, SerializationBuffer* packet)
{
	for (auto& users : g_sessionList)
	{
		if (users._dwSessionID != session->_dwSessionID)
		{
			sendPacket_Unicast(&users, packet);
		}
	}

	return true;
}

bool sendPacket_Around(st_SESSION* session, SerializationBuffer* packet)
{


	return true;
}

bool packetProc(st_SESSION* session, unsigned char packetType, SerializationBuffer* packet)
{
	switch (packetType)
	{
	case dfPACKET_CS_MOVE_START:
		netPacketProc_MoveStart(session, packet);
		break;
	case dfPACKET_CS_MOVE_STOP:
		netPacketProc_MoveStop(session, packet);
		break;
	case dfPACKET_CS_ATTACK1:
		netPacketProc_Attack1(session, packet);
		break;
	case dfPACKET_CS_ATTACK2:
		netPacketProc_Attack2(session, packet);
		break;
	case dfPACKET_CS_ATTACK3:
		netPacketProc_Attack3(session, packet);
		break;
	}

	return true;
}

bool netPacketProc_MoveStart(st_SESSION* session, SerializationBuffer* packet)
{
	unsigned char direction;
	short x;
	short y;

	*packet >> direction >> x >> y;

	if (abs(x - session->_shX) > dfERROR_RANGE || abs(y - session->_shY) > dfERROR_RANGE)
	{
		SerializationBuffer sendPacket;
		npfSync(&sendPacket, session->_dwSessionID, session->_shX, session->_shY);
		sendPacket_Around(session, &sendPacket);
	}

	session->_dwAction = direction;

	switch (direction)
	{
	case dfPACKET_MOVE_DIR_RR:
	case dfPACKET_MOVE_DIR_RU:
	case dfPACKET_MOVE_DIR_RD:
		session->_byDirection = dfPACKET_MOVE_DIR_RR;
		break;
	case dfPACKET_MOVE_DIR_LL:
	case dfPACKET_MOVE_DIR_LU:
	case dfPACKET_MOVE_DIR_LD:
		session->_byDirection = dfPACKET_MOVE_DIR_LL;
		break;
	}
	
	SerializationBuffer sendPacket;
	npfMoveStart(&sendPacket, session->_dwSessionID, session->_byDirection, session->_shX, session->_shY);
	sendPacket_Around(session, &sendPacket);

	return true;
}

bool netPacketProc_MoveStop(st_SESSION* session, SerializationBuffer* packet)
{
	return true;
}

bool netPacketProc_Attack1(st_SESSION* session, SerializationBuffer* packet)
{
	return true;
}

bool netPacketProc_Attack2(st_SESSION* session, SerializationBuffer* packet)
{
	return true;
}

bool netPacketProc_Attack3(st_SESSION* session, SerializationBuffer* packet)
{
	return true;
}

bool netIOProcess()
{
	fd_set rset;
	fd_set wset;

	FD_ZERO(&rset);
	FD_ZERO(&wset);

	FD_SET(g_listenSocket, &rset);

	for (auto iter = g_sessionList.begin(); iter != g_sessionList.end(); ++iter)
	{
		FD_SET(iter->_socket, &rset);
		if (iter->_sendQ.GetUseSize() > 0)
		{
			FD_SET(iter->_socket, &wset);
		}
	}
	
	timeval t;
	t.tv_sec = 0;
	t.tv_usec = 0;
	auto selectRet = select(0, &rset, &wset, NULL, &t);
	if (selectRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			printf("select : %d\n", WSAGetLastError());
			g_shutdown = true;
		}
		
		return false;
	}

	if (selectRet > 0)
	{
		if (FD_ISSET(g_listenSocket, &rset))
		{
			netProc_Accept();
		}

		for (auto& session : g_sessionList)
		{
			if (FD_ISSET(session._socket, &rset))
			{
				selectRet--;
				netProc_Recv(&session);
			}

			if (FD_ISSET(session._socket, &wset))
			{
				selectRet--;
				netProc_Send(&session);
			}
		}
	}

	return true;
}

bool Update()
{


	return true;
}

bool npfCreateCharacter(SerializationBuffer* packet, unsigned int id, unsigned char direction, short x, short y, char hp)
{
	st_HEADER header;
	header._byCode = ﻿dfNETWORK_PACKET_CODE;
	header._bySize = sizeof(st_SC_CREATE_MY_CHARACTER);
	header._byType = dfPACKET_SC_CREATE_MY_CHARACTER;

	packet->putData((char*)&header, sizeof(header));

	*packet << id;
	*packet << direction;
	*packet << x;
	*packet << y;
	*packet << hp;

	return true;
}

bool npfCreateOtherCharacter(SerializationBuffer* packet, unsigned int id, unsigned char direction, short x, short y, char hp)
{
	st_HEADER header;
	header._byCode = ﻿dfNETWORK_PACKET_CODE;
	header._bySize = sizeof(st_SC_CREATE_OTHER_CHARACTER);
	header._byType = dfPACKET_SC_CREATE_OTHER_CHARACTER;

	packet->putData((char*)&header, sizeof(header));

	*packet << id;
	*packet << direction;
	*packet << x;
	*packet << y;
	*packet << hp;

	return true;
}

bool npfDeleteCharacter(SerializationBuffer* packet, unsigned int id)
{
	st_HEADER header;
	header._byCode = ﻿dfNETWORK_PACKET_CODE;
	header._bySize = sizeof(st_SC_DELETE_CHARACTER);
	header._byType = dfPACKET_SC_DELETE_CHARACTER;

	packet->putData((char*)&header, sizeof(header));

	*packet << id;

	return true;
}

bool npfMoveStart(SerializationBuffer* packet, unsigned int id, unsigned char direction, short x, short y)
{
	st_HEADER header;
	header._byCode = ﻿dfNETWORK_PACKET_CODE;
	header._bySize = sizeof(st_SC_MOVE_START);
	header._byType = dfPACKET_SC_MOVE_START;

	packet->putData((char*)&header, sizeof(header));

	*packet << id;
	*packet << direction;
	*packet << x;
	*packet << y;

	return true;
}

bool npfMoveStop(SerializationBuffer* packet, unsigned int id, unsigned char direction, short x, short y)
{
	st_HEADER header;
	header._byCode = ﻿dfNETWORK_PACKET_CODE;
	header._bySize = sizeof(st_SC_MOVE_STOP);
	header._byType = dfPACKET_SC_MOVE_STOP;

	packet->putData((char*)&header, sizeof(header));

	*packet << id;
	*packet << direction;
	*packet << x;
	*packet << y;

	return true;
}

bool npfAttack1(SerializationBuffer* packet, unsigned int id, unsigned char direction, short x, short y)
{
	st_HEADER header;
	header._byCode = ﻿dfNETWORK_PACKET_CODE;
	header._bySize = sizeof(st_SC_ATTACK1);
	header._byType = dfPACKET_SC_ATTACK1;

	packet->putData((char*)&header, sizeof(header));

	*packet << id;
	*packet << direction;
	*packet << x;
	*packet << y;

	return true;
}

bool npfAttack2(SerializationBuffer* packet, unsigned int id, unsigned char direction, short x, short y)
{
	st_HEADER header;
	header._byCode = ﻿dfNETWORK_PACKET_CODE;
	header._bySize = sizeof(st_SC_ATTACK2);
	header._byType = dfPACKET_SC_ATTACK2;

	packet->putData((char*)&header, sizeof(header));

	*packet << id;
	*packet << direction;
	*packet << x;
	*packet << y;

	return true;
}

bool npfAttack3(SerializationBuffer* packet, unsigned int id, unsigned char direction, short x, short y)
{
	st_HEADER header;
	header._byCode = ﻿dfNETWORK_PACKET_CODE;
	header._bySize = sizeof(st_SC_ATTACK3);
	header._byType = dfPACKET_SC_ATTACK3;

	packet->putData((char*)&header, sizeof(header));

	*packet << id;
	*packet << direction;
	*packet << x;
	*packet << y;

	return true;
}

bool npfDamage(SerializationBuffer* packet, unsigned int attackID, unsigned int damageID, char damageHP)
{
	st_HEADER header;
	header._byCode = ﻿dfNETWORK_PACKET_CODE;
	header._bySize = sizeof(st_SC_DAMAGE);
	header._byType = dfPACKET_SC_DAMAGE;

	packet->putData((char*)&header, sizeof(header));

	*packet << attackID;
	*packet << damageID;
	*packet << damageHP;

	return true;
}

bool npfSync(SerializationBuffer* packet, unsigned int id, short x, short y)
{
	st_HEADER header;
	header._byCode = ﻿dfNETWORK_PACKET_CODE;
	header._bySize = sizeof(st_SC_SYNC);
	header._byType = dfPACKET_SC_SYNC;

	packet->putData((char*)&header, sizeof(header));

	*packet << id;
	*packet << x;
	*packet << y;

	return true;
}
