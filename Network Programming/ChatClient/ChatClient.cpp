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

list<SC_OTHER_USER> g_users;
USER g_user;
SOCKET g_serversock;
bool g_shutdown = false;

CRITICAL_SECTION cs;

bool netProc_Recv();
bool netProc_Send();
bool sendPacket_Unicast(SerializationBuffer* packet);
bool packetProc(unsigned char type, SerializationBuffer* packet);

bool networkLogic();
bool Update();

bool netPacketProc_CreateUser(SerializationBuffer* packet);
bool netPacketProc_OtherUser(SerializationBuffer* packet);
bool netPacketProc_MSG(SerializationBuffer* packet);

bool npfMSG(SerializationBuffer* packet, unsigned char len, char* msg);

unsigned int WINAPI threadProc(PVOID arg)
{
	while (!g_shutdown)
	{
		networkLogic();
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

	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, threadProc, NULL, NULL, NULL);
	if (hThread == 0)
	{
		return 1;
	}
	CloseHandle(hThread);

	while (!g_shutdown)
	{
		Update();
	}

	closesocket(g_serversock);
	WSACleanup();

	return 0;
}

bool netProc_Recv()
{
	if (g_user._recvQ.GetFreeSize() < sizeof(HEADER))
	{
		return false;
	}

	int recvRet = recv(g_serversock, g_user._recvQ.GetRearBufferPtr(),
		g_user._recvQ.DirectEnqueueSize(), 0);
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

	g_user._recvQ.MoveRear(recvRet);

	while (true)
	{
		if (g_user._recvQ.GetUseSize() <= sizeof(HEADER))
		{
			return false;
		}

		char buf[2];
		int peekRet = g_user._recvQ.Peek(buf, sizeof(HEADER));
		if (peekRet != sizeof(HEADER))
		{
			__debugbreak();
		}

		g_user._recvQ.MoveFront(sizeof(HEADER));

		HEADER* header = (HEADER*)buf;
		SerializationBuffer packet;
		peekRet = g_user._recvQ.Peek(packet.getBufferPtr(), header->_packetsize);
		if (peekRet != header->_packetsize)
		{
			__debugbreak();
		}
		g_user._recvQ.MoveFront(peekRet);
		packet.moveReadPos(peekRet);

		packetProc(header->_type, &packet);
	}

	return true;
}

bool netProc_Send()
{
	while (true)
	{
		if (g_user._sendQ.GetUseSize() < sizeof(HEADER))
		{
			break;
		}

		int sendRet = send(g_serversock, g_user._sendQ.GetFrontBufferPtr(),
			g_user._sendQ.DirectDequeueSize(), 0);
		if (sendRet == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				printf("%d\n", WSAGetLastError());
				printf("send fail\n");
			}
			break;
		}

		g_user._sendQ.MoveFront(sendRet);
	}

	return true;
}

bool sendPacket_Unicast(SerializationBuffer* packet)
{
	EnterCriticalSection(&cs);
	int size = packet->getDataSize();
	if (g_user._sendQ.GetFreeSize() < size)
	{
		printf("send fail\n");
		return false;
	}

	int enqueueRet = g_user._sendQ.Enqueue(packet->getBufferPtr(), size);
	if (enqueueRet != packet->getDataSize())
	{
		__debugbreak();
	}

	packet->moveWritePos(size);
	LeaveCriticalSection(&cs);

	return true;
}

bool packetProc(unsigned char type, SerializationBuffer* packet)
{
	switch (type)
	{
	case PACKET_SC_CREATE_USER:
		netPacketProc_CreateUser(packet);
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

bool networkLogic()
{
	fd_set rset;
	fd_set wset;
	FD_ZERO(&rset);
	FD_ZERO(&wset);

	FD_SET(g_serversock, &rset);
	if (g_user._sendQ.GetUseSize() > 0)
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
			netProc_Recv();
		}

		if (FD_ISSET(g_serversock, &wset))
		{
			selectRet--;
			netProc_Send();
		}
	}

	return true;
}

bool Update()
{
	char msg[200];
	if (fgets(msg, 200, stdin) != NULL)
	{
		unsigned char len = (unsigned char)strlen(msg);
		msg[len - 1] = '\0';

		SerializationBuffer packet;
		npfMSG(&packet, len, msg);
		sendPacket_Unicast(&packet);
	}

	return true;
}

bool netPacketProc_CreateUser(SerializationBuffer* packet)
{
	*packet >> g_user._id;
	*packet >> g_user._namesize;
	packet->getData(g_user._name, g_user._namesize);

	return true;
}

bool netPacketProc_OtherUser(SerializationBuffer* packet)
{
	SC_OTHER_USER otherUser;
	*packet >> otherUser._id;
	*packet >> otherUser._namesize;
	packet->getData(otherUser._name, otherUser._namesize);
	g_users.push_back(otherUser);

	return true;
}

bool netPacketProc_MSG(SerializationBuffer* packet)
{
	unsigned char len;
	char msg[200];

	*packet >> len;
	packet->getData(msg, len);

	printf("%s\n", msg);

	return true;
}

bool npfMSG(SerializationBuffer* packet, unsigned char len, char* msg)
{
	HEADER header;
	header._packetsize = sizeof(len) + len;
	header._type = PACKET_CS_MSG;

	packet->putData((char*)&header, sizeof(HEADER));

	*packet << len;
	packet->putData(msg, len);

	return true;
}
