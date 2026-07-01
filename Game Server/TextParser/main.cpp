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


	parser->DestoryInstance();

	return 0;
}