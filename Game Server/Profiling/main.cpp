#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <Windows.h>
using namespace std;

//#include "Profile.h"

#ifdef PROFILE
	#define PRO_BEGIN(TagName)	ProfileBegin(TagName)
	#define PRO_END(TagName)	ProfileEnd(TagName)
#else
	#define PRO_BEGIN(TagName)	OUT
	#define PRO_END(TagName)	OUT
#endif

#pragma comment(lib, "winmm.lib")


int main()
{
	timeBeginPeriod(1);

	LARGE_INTEGER start;
	LARGE_INTEGER end;
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);

	QueryPerformanceCounter(&start);
	Sleep(1000);
	QueryPerformanceCounter(&end);

	cout << "second : " << (double)(end.QuadPart - start.QuadPart) / freq.QuadPart << "\n";

	return 0;
}