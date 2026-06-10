#include <iostream>
#include <Windows.h>
using namespace std;

#pragma comment(lib, "winmm.lib")

char buffer[4];

int main()
{
	timeBeginPeriod(1);

	SYSTEM_INFO systemInfo;
	MEMORY_BASIC_INFORMATION mbi;
	int pid;
	int findNum;

	cout << "Process ID : ";
	cin >> pid;

	cout << "Find Value : ";
	cin >> findNum;

	GetSystemInfo(&systemInfo);

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid); 
	void* addr = systemInfo.lpMinimumApplicationAddress;

	while (true)
	{
		VirtualQueryEx(hProcess, addr, &mbi, sizeof(mbi));
		if (systemInfo.lpMaximumApplicationAddress < addr)
		{
			break;
		}

		cout << "BaseAddress : [" << hex << mbi.BaseAddress << "]" << "\n";
		cout << "Page State : ";
		if (mbi.State == MEM_COMMIT)
		{
			cout << "MEM_COMMIT" << "\n";
		}
		else if (mbi.State == MEM_RESERVE)
		{
			cout << "MEM_RESERVE" << "\n";
		}
		else if (mbi.State == MEM_FREE)
		{
			cout << "MEM_FREE" << "\n";
		}

		int size = 0;
		for (; size + 4 <= mbi.RegionSize; size += 4)
		{
			ReadProcessMemory(hProcess, (BYTE*)mbi.BaseAddress + size, buffer, 4, NULL);

			if (mbi.Type == MEM_PRIVATE && mbi.State == MEM_COMMIT)
			{
				if (memcmp(&findNum, buffer, sizeof(int)) == 0)
				{
					int changeValue = 0;
					cout << "Change Value : ";
					cin >> changeValue;

					WriteProcessMemory(hProcess, (BYTE*)mbi.BaseAddress + size, &changeValue, 4, NULL);
				}
			}
			else
			{
				break;
			}
		}

		addr = (BYTE*)mbi.BaseAddress + mbi.RegionSize;
		cout << "\n\n";
	}

	return 0;
}