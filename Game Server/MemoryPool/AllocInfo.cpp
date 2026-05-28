#include "AllocInfo.h"
#undef new
#include <iostream>
#include <string>
#include <time.h>
using namespace std;

MemoryPool memoryPool;
char title[256];
char fileLog[256][256];
static unsigned int fileIndex = 0;
static unsigned int allocIndex = 0;

MemoryPool::MemoryPool()
{}

MemoryPool::~MemoryPool()
{
	// 로그 파일 작성
	fileTitle();

	for (unsigned int i = 0; i < allocIndex; ++i)
	{
		if (allocInfo[i].ptr != nullptr)
		{
			// LEAK 로그 작성
			char arr[256] = { '\0', };
			strcat_s(arr, sizeof(arr), "LEAK ");
			strcat_s(arr, sizeof(arr), "[");
			strcat_s(arr, sizeof(arr), to_string((int)allocInfo[i].ptr).c_str());
			strcat_s(arr, sizeof(arr), "] ");
			strcat_s(arr, sizeof(arr), "[");
			strcat_s(arr, sizeof(arr), to_string(allocInfo[i].size).c_str());
			strcat_s(arr, sizeof(arr), "] ");
			strcat_s(arr, sizeof(arr), allocInfo[i].fileName);
			strcat_s(arr, sizeof(arr), " : ");
			strcat_s(arr, sizeof(arr), to_string(allocInfo[i].line).c_str());

			strcpy_s(fileLog[fileIndex], sizeof(fileLog[fileIndex]), arr);
			++fileIndex;
		}
	}

	FILE* fp;
	fopen_s(&fp, title, "wb");
	for (unsigned int i = 0; i < fileIndex; ++i)
	{
		fwrite(fileLog[i], sizeof(char), strlen(fileLog[i]), fp);
		fwrite("\n", sizeof(char), 1, fp);
	}

	fclose(fp);
}

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

void MemoryPool::fileTitle()
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

		strcat_s(title, sizeof(title), "Alloc_");
		strcat_s(title, sizeof(title), to_string(year).c_str());
		strcat_s(title, sizeof(title), to_string(month).c_str());
		strcat_s(title, sizeof(title), to_string(day).c_str());
		strcat_s(title, sizeof(title), "_");
		strcat_s(title, sizeof(title), to_string(hour).c_str());
		strcat_s(title, sizeof(title), to_string(minute).c_str());
		strcat_s(title, sizeof(title), to_string(second).c_str());
		strcat_s(title, sizeof(title), ".txt");
	}
}

void MemoryPool::AddAlloc(void* ptr, size_t size, const char* fileName, int line, bool array)
{
	AllocInfo info;
	info.ptr = ptr;
	info.size = size;
	strcpy_s(info.fileName, sizeof(info.fileName), fileName);
	info.line = line;
	info.array = array;
	allocInfo[allocIndex] = info;
	++allocIndex;
}

bool MemoryPool::RemoveAlloc(void* ptr, bool array)
{
	void* findPtr = nullptr;

	int cnt = 0;
	for (unsigned int i = 0; i < allocIndex; ++i)
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
		char str[256] = {'\0', };
		strcat_s(str, sizeof(str), "NOALLOC ");
		strcat_s(str, sizeof(str), "[");
		strcat_s(str, sizeof(str), to_string((int)ptr).c_str());
		strcat_s(str, sizeof(str), "]");

		strcpy_s(fileLog[fileIndex], sizeof(fileLog[fileIndex]), str);
		++fileIndex;

		return 0;
	}

	if (allocInfo[cnt].array != array)
	{
		// ARRAY 로그 작성
		char str[256] = { '\0', };
		strcat_s(str, sizeof(str), "ARRAY ");
		strcat_s(str, sizeof(str), "[");
		strcat_s(str, sizeof(str), to_string((int)ptr).c_str());
		strcat_s(str, sizeof(str), "] ");
		strcat_s(str, sizeof(str), "[");
		strcat_s(str, sizeof(str), to_string(allocInfo[cnt].size).c_str());
		strcat_s(str, sizeof(str), "] ");
		strcat_s(str, sizeof(str), allocInfo[cnt].fileName);
		strcat_s(str, sizeof(str), " : ");
		strcat_s(str, sizeof(str), to_string(allocInfo[cnt].line).c_str());

		strcpy_s(fileLog[fileIndex], sizeof(fileLog[fileIndex]), str);
		++fileIndex;
	}
	
	allocInfo[cnt].ptr = nullptr;
	return 1;
}
