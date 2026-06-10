#include "overflow_check.h"
#include <Windows.h>

void* alloc_overflow_check(int size)
{
	int totalPage = 1024 * 64;
	while (true)
	{
		if (totalPage <= size)
		{
			totalPage *= 2;
		}
		else
		{
			break;
		}
	}

	int onePage = 1024 * 4;
	int rangePage = (size + 4095) & ~4095;

	auto vp = VirtualAlloc(0, totalPage, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	char* ptr = (char*)vp;
	for (auto i = 0x0000; i < rangePage; i += 0x1000)
	{
		ptr += onePage;
	}

	DWORD value;
	auto isSuccess = VirtualProtect(ptr, onePage, PAGE_NOACCESS, &value);
	if (!isSuccess)
	{
		return nullptr;
	}

	// 페이지 4KB 시작 부분 맞추기
	/*
	int remainPage = (size % rangePage);
	ptr += (1024 * remainPage);
	*/

	ptr -= size;
	return ptr;
}

void free_overflow_check(void* ptr, int size)
{
	int onePage = 1024 * 4;
	int rangePage = (size + 4095) & ~4095;

	char* p = (char*)ptr;
	p += size;

	for (auto i = 0x0000; i < rangePage; i += 0x1000)
	{
		p -= onePage;
	}

	// 페이지 4KB 시작 부분 맞추기
	/*
	int remainPage = (size % rangePage);
	p -= (1024 * remainPage);
	*/

	VirtualFree(p, 0, MEM_RELEASE);
}