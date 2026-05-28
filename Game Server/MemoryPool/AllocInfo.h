#pragma once
extern char fileLog[256][256];

// 64비트 환경에서 로그에 찍을 때,
// 주소 8바이트로 변경해야됨
// int -> long long || __int64

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

	void fileTitle();

	void AddAlloc(void* ptr, size_t size, const char* fileName, int line, bool array);
	bool RemoveAlloc(void* ptr, bool array);

public:
	AllocInfo allocInfo[256];
};

extern MemoryPool memoryPool;

void* operator new(size_t size, const char* fileName, int line);
void* operator new[](size_t size, const char* fileName, int line);

void operator delete(void* ptr);
void operator delete[](void* ptr);

#define new new(__FILE__, __LINE__)