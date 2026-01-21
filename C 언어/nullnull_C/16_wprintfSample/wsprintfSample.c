#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	wchar_t* pwszData = L"String";
	wchar_t wszData[12];

	wcscpy_s(wszData, _countof(wszData), pwszData);
	wprintf(L"%s\n", wszData);
	return 0;
}