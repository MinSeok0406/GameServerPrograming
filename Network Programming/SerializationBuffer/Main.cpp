#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <time.h>
#include <fcntl.h>
#include <io.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <list>
#include "SerializationBuffer.h"
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

void InputTest(SerializationBuffer* packet, int x, int y)
{
	*packet << x << y;
}

void OutputTest(SerializationBuffer* packet)
{
	int x;
	int y;

	*packet >> x >> y;

	cout << x << " " << y << "\n";
}

int wmain()
{
	srand((unsigned int)time(nullptr));
	
	SerializationBuffer srBuffer(5000);
	InputTest(&srBuffer, 45, 50);

	OutputTest(&srBuffer);

	return 0;
}