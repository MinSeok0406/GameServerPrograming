#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <Windows.h>
using namespace std;

#include "Profile.h"

#ifdef PROFILE
	#define PRO_BEGIN(TagName)	ProfileBegin(TagName)
	#define PRO_END(TagName)	ProfileEnd(TagName)
#else
	#define PRO_BEGIN(TagName)	OUT
	#define PRO_END(TagName)	OUT
#endif

#pragma comment(lib, "winmm.lib")

void Test()
{
	PRO_BEGIN(L"Test");

	auto t = (rand() % 1000) + 1000;
	Sleep(t);

	PRO_END(L"Test");
}

int wmain()
{
	timeBeginPeriod(1);
	srand((unsigned int)time(nullptr));

	for (auto i = 0; i < 30; i++)
	{
		Test();
	}

	ProfileDataOutText(L"profile.txt");

	return 0;
}