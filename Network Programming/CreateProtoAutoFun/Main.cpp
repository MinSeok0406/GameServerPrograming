#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <time.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <windowsx.h>
#include <list>
#include <string>
#include <algorithm>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

char funcName[50];
char parameterName[200];
string pstr[10];

int getFuncName(char* buf);
int getParameterName(char* buf);
bool getParameter();

// 매개변수 변수명도 따로 뽑아서 관리 -> 각 기본 타입 다음에 나오는 값을 ,가 나오기까지 추출
int wmain()
{
	FILE* fp;
	fopen_s(&fp, "proto.txt", "rb");
	if (fp == nullptr)
	{
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char buf[1500];
	auto readRet = fread(buf, fileSize, 1, fp);
	buf[fileSize] = '\0';
	fclose(fp);

	char* bbuf = buf;
	int readpos = 0;
	while (*bbuf != '\0')
	{
		memset(funcName, 0, sizeof(funcName));
		memset(parameterName, 0, sizeof(parameterName));
		readpos = getFuncName(bbuf) + 1;
		bbuf += readpos;
		readpos = getParameterName(bbuf) + 1;
		bbuf += readpos;

		if (*bbuf == '\r')
		{
			bbuf += 2;
		}

		for (auto& s : pstr)
		{
			s.clear();
		}

		static int number = 0;
		const char* protoStr =
			"#define dfNETWORK_PACKET_CODE 0x89\n"
			"#define dfPACKET_SC_%s	%d\n"
			"bool npf%s(SerializationBuffer* packet, %s)\n"
			"{\n"
			"\tst_HEADER header;\n"
			"\theader._byCode = dfNETWORK_PACKET_CODE;\n"
			"\theader._byType = dfPACKET_SC_%s;\n"
			"\n"
			"\tpacket->setHeaderSize(sizeof(st_HEADER))\n";

		char result[2000];
		sprintf_s(result, 2000, protoStr, funcName, number++, funcName, parameterName, funcName);

		getParameter();

		for (auto& s : pstr)
		{
			if (s.empty())
			{
				break;
			}

			char line[128];
			sprintf_s(line, 128, "\t*packet << %s;\n", s.c_str());
			strcat_s(result, 2000, line);
		}

		strcat_s(result, 2000, "\n");
		strcat_s(result, 2000, "\theader._bySize = packet->getDataSize() - sizeof(st_HEADER);\n");
		strcat_s(result, 2000, "\tpacket->headerWritePos();\n");
		strcat_s(result, 2000, "\tpacket->putData((char*)&header, sizeof(header));\n");
		strcat_s(result, 2000, "\tpacket->posReset();\n");
		strcat_s(result, 2000, "}\n\n");

		FILE* fp;
		fopen_s(&fp, "Test.cpp", "ab");
		if (fp == nullptr)
		{
			break;
		}

		int len = strlen(result);
		int writeRet = fwrite(result, len, 1, fp);
		fclose(fp);
	}

	return 0;
}

int getFuncName(char* buf)
{
	int index = 0;
	while (*buf != '(')
	{
		funcName[index] = *buf;
		buf++;
		index++;
	}

	return index;
}

int getParameterName(char* buf)
{
	int index = 0;
	while (*buf != ')')
	{
		parameterName[index] = *buf;
		buf++;
		index++;
	}

	return index;
}

bool getParameter()
{
	char* parameter = parameterName;
	char* context = nullptr;
	char* token = strtok_s(parameter, ",", &context);

	int index = 0;
	while (token != nullptr)
	{
		while (*token == ' ')
		{
			token++;
		}

		char* lastSpace = strrchr(token, ' ');

		if (lastSpace != nullptr)
		{
			*lastSpace = '\0';
			pstr[index] = lastSpace + 1;
			index++;
		}

		token = strtok_s(nullptr, ",", &context);
	}

	return true;
}