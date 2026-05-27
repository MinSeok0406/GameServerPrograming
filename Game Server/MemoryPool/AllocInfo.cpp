#include "AllocInfo.h"
using namespace std;

MemoryPool memoryPool;
vector<string> fileLog;

#undef new

void* operator new(size_t size, const char* fileName, int line)
{
	void* ptr = malloc(size);
	memoryPool.AddAlloc(ptr, size, fileName, line, false);
	return ptr;
}

void* operator new[](size_t size, const char* fileName, int line)
{
	void* ptr = malloc(size);
	memoryPool.AddAlloc(ptr, size, fileName, line, true);
	return ptr;
}

void operator delete(void* ptr)
{
	if (memoryPool.RemoveAlloc(ptr, false))
	{
		free(ptr);
	}
}

void operator delete[](void* ptr)
{
	if (memoryPool.RemoveAlloc(ptr, true))
	{
		free(ptr);
	}
}

string MemoryPool::fileTitle()
{
	string str;
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

		str += "Alloc_";
		str += to_string(year);
		str += to_string(month);
		str += to_string(day);
		str += "_";
		str += to_string(hour);
		str += to_string(minute);
		str += to_string(second);
		str += ".txt";
	}

	return str;
}

void MemoryPool::AddAlloc(void* ptr, size_t size, const char* fileName, int line, bool array)
{
	AllocInfo info;
	info.ptr = ptr;
	info.size = size;
	strcpy_s(info.fileName, fileName);
	info.line = line;
	info.array = false;
	allocInfo.push_back(info);
}

bool MemoryPool::RemoveAlloc(void* ptr, bool array)
{
	void* findPtr = nullptr;

	int cnt = 0;
	for (auto i = 0; i < (int)allocInfo.size(); ++i)
	{
		if (allocInfo[i].ptr == ptr)
		{
			findPtr = allocInfo[i].ptr;
			cnt = i;
			break;
		}
	}

	if (findPtr == nullptr)
	{
		// NOALLOC 로그 작성
		string str;
		str += "NOALLOC ";
		str += "[";
		str += to_string((int)&ptr);
		str += "]";

		fileLog.push_back(str);

		return 0;
	}

	if (allocInfo[cnt].array != array)
	{
		// ARRAY 로그 작성
		string str;
		str += "ARRAY ";
		str += "[";
		str += to_string((int)&ptr);
		str += "] ";
		str += "[";
		str += to_string(allocInfo[cnt].size);
		str += "] ";
		str += allocInfo[cnt].fileName;
		str += " : ";
		str += to_string(allocInfo[cnt].line);

		fileLog.push_back(str);

		return 0;
	}

	return 1;
}
