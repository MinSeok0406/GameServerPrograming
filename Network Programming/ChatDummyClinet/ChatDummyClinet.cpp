#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <time.h>
#include <list>
#include <string>
#include <algorithm>
#include "Protocol.h"
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

const wchar_t* SERVERIP = L"127.0.0.1";
#define SERVERPORT 47000
#define CLIENT 100

SOCKET g_serversock;
bool g_shutdown = false;

CRITICAL_SECTION cs;

bool netProc_Recv(USER* user);
bool netProc_Send(USER* user);
bool sendPacket_Unicast(SerializationBuffer* packet, USER* user);
bool packetProc(unsigned char type, SerializationBuffer* packet, USER* user);

bool networkLogic(USER* user);
bool Update(USER* user);

bool netPacketProc_CreateUser(SerializationBuffer* packet, USER* user);
bool netPacketProc_OtherUser(SerializationBuffer* packet);
bool netPacketProc_MSG(SerializationBuffer* packet);

bool npfMSG(SerializationBuffer* packet, unsigned short len, unsigned int namesize, char name[20], char* msg);

unsigned int WINAPI threadProc(PVOID arg)
{
	USER user;

	while (!g_shutdown)
	{
		networkLogic(&user);
		Update(&user);
		Sleep(10);
	}

	return 1;
}

int wmain()
{
	srand((unsigned int)time(nullptr));
	timeBeginPeriod(1);

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}

	InitializeCriticalSection(&cs);
	g_serversock = socket(AF_INET, SOCK_STREAM, 0);
	if (g_serversock == INVALID_SOCKET)
	{
		printf("%d\n", WSAGetLastError());
		return 1;
	}

	SOCKADDR_IN serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	InetPton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);

	int connectRet = connect(g_serversock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (connectRet == SOCKET_ERROR)
	{
		printf("%d\n", WSAGetLastError());
		return 1;
	}

	u_long on = 1;
	int nonblkRet = ioctlsocket(g_serversock, FIONBIO, &on);
	if (nonblkRet == SOCKET_ERROR)
	{
		printf("%d\n", WSAGetLastError());
		return 1;
	}

	HANDLE hThread[CLIENT];
	for (auto i = 0; i < CLIENT; ++i)
	{
		hThread[i] = (HANDLE)_beginthreadex(NULL, 0, threadProc, NULL, NULL, NULL);
		if (hThread == 0)
		{
			return 1;
		}
		CloseHandle(hThread[i]);
	}

	while (!g_shutdown)
	{
		
	}

	closesocket(g_serversock);
	WSACleanup();

	return 0;
}

bool netProc_Recv(USER* user)
{
	if (user->_recvQ.GetFreeSize() < sizeof(HEADER))
	{
		return false;
	}

	int recvRet = recv(g_serversock, user->_recvQ.GetRearBufferPtr(),
		user->_recvQ.DirectEnqueueSize(), 0);
	if (recvRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			printf("%d\n", WSAGetLastError());
			printf("recv fail\n");
		}

		return false;
	}
	else if (recvRet == 0)
	{
		g_shutdown = true;
		return true;
	}

	user->_recvQ.MoveRear(recvRet);

	while (true)
	{
		if (user->_recvQ.GetUseSize() <= sizeof(HEADER))
		{
			return false;
		}

		char buf[3];
		int peekRet = user->_recvQ.Peek(buf, sizeof(HEADER));
		if (peekRet != sizeof(HEADER))
		{
			__debugbreak();
		}

		user->_recvQ.MoveFront(sizeof(HEADER));

		HEADER* header = (HEADER*)buf;
		SerializationBuffer packet;
		peekRet = user->_recvQ.Peek(packet.getBufferPtr(), header->_packetsize);
		if (peekRet != header->_packetsize)
		{
			__debugbreak();
		}
		user->_recvQ.MoveFront(peekRet);
		packet.moveReadPos(peekRet);

		packetProc(header->_type, &packet, user);
	}

	return true;
}

bool netProc_Send(USER* user)
{
	while (true)
	{
		if (user->_sendQ.GetUseSize() < sizeof(HEADER))
		{
			break;
		}

		int sendRet = send(g_serversock, user->_sendQ.GetFrontBufferPtr(),
			user->_sendQ.DirectDequeueSize(), 0);
		if (sendRet == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				printf("%d\n", WSAGetLastError());
				printf("send fail\n");
			}
			break;
		}

		user->_sendQ.MoveFront(sendRet);
	}

	return true;
}

bool sendPacket_Unicast(SerializationBuffer* packet, USER* user)
{
	EnterCriticalSection(&cs);
	int size = packet->getDataSize();
	if (user->_sendQ.GetFreeSize() < size)
	{
		printf("send fail\n");
		LeaveCriticalSection(&cs);
		return false;
	}

	int enqueueRet = user->_sendQ.Enqueue(packet->getBufferPtr(), size);
	if (enqueueRet != packet->getDataSize())
	{
		__debugbreak();
	}

	packet->moveWritePos(size);
	LeaveCriticalSection(&cs);

	return true;
}

bool packetProc(unsigned char type, SerializationBuffer* packet, USER* user)
{
	switch (type)
	{
	case PACKET_SC_CREATE_USER:
		netPacketProc_CreateUser(packet, user);
		break;
	case PACKET_SC_OTHER_USER:
		netPacketProc_OtherUser(packet);
		break;
	case PACKET_SC_MSG:
		netPacketProc_MSG(packet);
		break;
	}

	return true;
}

bool networkLogic(USER* user)
{
	fd_set rset;
	fd_set wset;
	FD_ZERO(&rset);
	FD_ZERO(&wset);

	FD_SET(g_serversock, &rset);
	if (user->_sendQ.GetUseSize() > 0)
	{
		FD_SET(g_serversock, &wset);
	}

	timeval t;
	t.tv_sec = 0;
	t.tv_usec = 0;

	int selectRet = select(0, &rset, &wset, nullptr, &t);
	if (selectRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			printf("%d\n", WSAGetLastError());
		}

		return false;
	}

	if (selectRet > 0)
	{
		if (FD_ISSET(g_serversock, &rset))
		{
			selectRet--;
			netProc_Recv(user);
		}

		if (FD_ISSET(g_serversock, &wset))
		{
			selectRet--;
			netProc_Send(user);
		}
	}

	return true;
}

bool Update(USER* user)
{
	const char* msg = "안녕하세요!!";
	unsigned char len = (unsigned char)strlen(msg);

	SerializationBuffer packet;
	npfMSG(&packet, len, user->_namesize, user->_name, (char*)msg);
	sendPacket_Unicast(&packet, user);

	return true;
}

bool netPacketProc_CreateUser(SerializationBuffer* packet, USER* user)
{
	SC_CREATE_USER createUser;
	*packet >> createUser._id;
	*packet >> createUser._namesize;
	packet->getData(createUser._name, createUser._namesize);

	user->_id = createUser._id;
	user->_namesize = createUser._namesize;
	memcpy(user->_name, createUser._name, createUser._namesize);

	return true;
}

bool netPacketProc_OtherUser(SerializationBuffer* packet)
{
	SC_OTHER_USER otherUser;
	*packet >> otherUser._id;
	*packet >> otherUser._namesize;
	packet->getData(otherUser._name, otherUser._namesize);

	return true;
}

bool netPacketProc_MSG(SerializationBuffer* packet)
{
	unsigned short len;
	unsigned int namesize;
	char name[20];
	char msg[500];

	*packet >> len;
	*packet >> namesize;
	packet->getData(name, namesize);
	packet->getData(msg, len);

	printf("%s:%s\n", name, msg);

	return true;
}

bool npfMSG(SerializationBuffer* packet, unsigned short len, unsigned int namesize, char name[20], char* msg)
{
	HEADER header;
	header._packetsize = (unsigned short)(sizeof(len) + sizeof(namesize) + namesize + len);
	header._type = PACKET_CS_MSG;

	packet->putData((char*)&header, sizeof(HEADER));

	*packet << len;
	*packet << namesize;
	packet->putData(name, namesize);
	packet->putData(msg, len);

	return true;
}
