#pragma once
#include <iostream>
#include <time.h>
#include <vector>
#include <string>

extern std::vector<std::string> fileLog;

struct AllocInfo
{
	void* ptr;
	int size;
	char fileName[256];
	int line;
	bool array;
};

class MemoryPool
{
public:
	MemoryPool()
	{

	}

	~MemoryPool()
	{
		// 로그 파일 작성
		std::string str = fileTitle();

		for (const auto& i : allocInfo)
		{
			if (i.ptr != nullptr)
			{
				// LEAK 로그 작성
				std::string arr;
				arr += "LEAK ";
				arr += "[";
				arr += std::to_string((int)&i.ptr);
				arr += "] ";
				arr += "[";
				arr += std::to_string(i.size);
				arr += "] ";
				arr += i.fileName;
				arr += " : ";
				arr += std::to_string(i.line);

				fileLog.push_back(arr);
			}
		}

		FILE* fp;
		fopen_s(&fp, str.c_str(), "wb");
		for (const auto& i : fileLog)
		{
			char* buffer = nullptr;
			memcpy_s(buffer, sizeof(buffer), i.c_str(), sizeof(i.c_str()));
			auto size = strlen(buffer);

			fwrite(buffer, sizeof(char), size, fp);
		}

		fclose(fp);
	}

	std::string fileTitle();

	void AddAlloc(void* ptr, size_t size, const char* fileName, int line, bool array);
	bool RemoveAlloc(void* ptr, bool array);

public:
	std::vector<AllocInfo> allocInfo;
};

extern MemoryPool memoryPool;

void* operator new(size_t size, const char* fileName, int line);
void* operator new[](size_t size, const char* fileName, int line);

void operator delete(void* ptr);
void operator delete[](void* ptr);

#define new new(__FILE__, __LINE__)