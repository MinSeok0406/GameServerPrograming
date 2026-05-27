#include "AllocInfo.h"

MemoryPool g_memorypool;

MemoryPool::MemoryPool() : _allocinfo({nullptr, 0, "", 0, false})
{

}

MemoryPool::~MemoryPool()
{

}

void* MemoryPool::operator new(size_t size, char* fileName = 0x00, int line = 0)
{


	return nullptr;
}

void* MemoryPool::operator new[](size_t size, char* fileName = 0x00, int line = 0)
{


	return nullptr;
}

void MemoryPool::operator delete(void* ptr, char* fileName, int line)
{
}

void MemoryPool::operator delete[](void* ptr, char* fileName, int line)
{
}
