#include <stdio.h>
#include <time.h>

int main(void)
{
	struct tm curtime = { 0 };
	time_t t = 0;

	t = time(NULL);
	localtime_s(&curtime, &t);

	printf("%lld\n", t);

	char szBuffer[128] = { 0 };
	ctime_s(szBuffer, sizeof(szBuffer), & t);
	printf("%s", szBuffer);

	printf("%04d-%02d-%02d\n",
		curtime.tm_year + 1900,
		curtime.tm_mon + 1, curtime.tm_mday);
	return 0;
}