#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <time.h>
#include <fcntl.h>
#include <io.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <list>
#include "RingBuffer.h"
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

int wmain()
{
	srand((unsigned int)time(nullptr));
	RingBuffer rb(723);

	const char* buf = "111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999";
	char inbuf[200];
	char outbuf[90];

	memset(inbuf, 0, sizeof(inbuf));
	memset(outbuf, 0, sizeof(outbuf));

	int size = strlen(buf);
	int sum = 0;
	int recvByte = 0;

	while (true)
	{
		// Enqueue
		char* temp = (char*)(buf + sum);
		int innum = rand() % 20;
		int originnum = innum;
		if (temp + innum > &buf[size])
		{
			int a = (int)(&buf[size] - temp);
			memcpy(inbuf, temp, a);
			memcpy(inbuf + a, buf, innum - a);
		}
		else
		{
			memcpy(inbuf, temp, innum);
		}

		if (rb.Enqueue(inbuf, innum) != 0)
		{
			sum += originnum;
			if (sum >= size)
			{
				sum %= size;
			}
		}

		// Dequeue
		int outnum = rand() % 20;
		if (recvByte + outnum >= 90)
		{
			int remain = 90 - recvByte;

			if (rb.Dequeue(outbuf + recvByte, remain) != 0)
			{
				recvByte = 0;

				if (memcmp(buf, outbuf, 90) != 0)
				{
					__debugbreak();
				}
				memset(outbuf, 0, sizeof(outbuf));
				if (rb.Dequeue(outbuf, outnum - remain) != 0)
				{
					recvByte += outnum - remain;
				}
			}

			continue;
		}
		else
		{
			if (rb.Dequeue(outbuf + recvByte, outnum) != 0)
			{
				recvByte += outnum;
			}
		}
	}

	return 0;
}