#include "AllocInfo.h"
#include <vector>
using namespace std;

vector<AllocInfo> allocinfo;

void* operator new(size_t size, const char* fileName = 0x00, int line = 0x00)
{
	void* ptr = malloc(size);
	AllocInfo info;
	info.ptr = ptr;
	info.size = size;
	strcpy_s(info.fileName, fileName);
	info.line = line;
	info.array = false;

	allocinfo.push_back(info);

	return ptr;
}

void* operator new[](size_t size, const char* fileName = 0x00, int line = 0x00)
{
	void* ptr = malloc(size);
	AllocInfo info;
	info.ptr = ptr;
	info.size = size;
	strcpy_s(info.fileName, fileName);
	info.line = line;
	info.array = true;

	allocinfo.push_back(info);

	return ptr;
}

void operator delete(void* ptr)
{



	free(ptr);
}

void operator delete[](void* ptr)
{



	free(ptr);
}
