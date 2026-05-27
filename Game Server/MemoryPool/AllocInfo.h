#pragma once

struct AllocInfo
{
	void* ptr;
	int size;
	char fileName[256];
	int line;
	bool array;
};

void* operator new(size_t size, const char* fileName, int line);
void* operator new[](size_t size, const char* fileName, int line);

void operator delete(void* ptr);
void operator delete[](void* ptr);