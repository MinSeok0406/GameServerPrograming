#include <iostream>
#include <time.h>
#include <memory>
#include <Windows.h>
//#include "AllocInfo.h"
using namespace std;
using ll = long long;

void fileTitle()
{
	time_t timer = time(nullptr);
	tm time_info;

	auto err = localtime_s(&time_info, &timer);
	if (err == 0)
	{
		int year = time_info.tm_year + 1900;
		int month = time_info.tm_mon + 1;
		int day = time_info.tm_mday;
		int hour = time_info.tm_hour;
		int minute = time_info.tm_min;
		int second = time_info.tm_sec;

		cout << year << " " << month << " " << day << " " << hour << " "
			<< minute << " " << second << "\n";
	}
}

int main()
{
	int* ptr = new(1, __FILE__, __LINE__) int;


	delete ptr;


	return 0;
}