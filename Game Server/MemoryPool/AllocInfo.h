#pragma once
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
	MemoryPool();
	~MemoryPool();

	void* operator new(size_t size, char* fileName, int line);
	void* operator new[](size_t size, char* fileName, int line);

	void operator delete(void* ptr, char* fileName, int line);
	void operator delete[](void* ptr, char* fileName, int line);

private:
	AllocInfo _allocinfo;
};