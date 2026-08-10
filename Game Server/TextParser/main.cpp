#include <iostream>
#include <Windows.h>
#include "Parser.h"
using namespace std;

#pragma comment(lib, "winmm.lib")

Parser* parser = nullptr;

int wmain()
{
	timeBeginPeriod(1);

	parser = Parser::GetInstance();

	if (parser->LoadFile(L"Config.txt"))
	{
		int version = 0;
		int serverID = 0;
		int port = 0;

		parser->GetValue(L"Version", &version);
		parser->GetValue(L"ServerID", &serverID);
		parser->GetValue(L"ServerBindPort", &port);

		wcout << L"Version = " << version << endl;
		wcout << L"ServerID = " << serverID << endl;
		wcout << L"ServerBindPort = " << port << endl;
	}
	else
	{
		wcout << L"Config.cnf 파일을 열지 못했습니다." << endl;
	}

	parser->DestoryInstance();

	return 0;
}