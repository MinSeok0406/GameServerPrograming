#include <cstdlib>
#include <cstring>
#include "RingBuffer.h"

RingBuffer::RingBuffer() : buf(nullptr), size(0), writePos(nullptr), readPos(nullptr)
{
	
}

RingBuffer::RingBuffer(int pBufferSize)
{
	buf = (char*)malloc(pBufferSize);
	size = pBufferSize;
	writePos = &buf[0];
	readPos = &buf[0];
}

RingBuffer::~RingBuffer()
{
	free(buf);
	writePos = nullptr;
	readPos = nullptr;
}

int RingBuffer::GetUseSize()
{
	char* start = readPos;
	char* end = writePos;

	int useSize = 0;
	// true : read와 write가 한 경계에 존재할 때
	// false : read와 write가 경계면 사이에 존재할 때
	if (start <= end)
	{
		useSize = (int)(end - start);
	}
	else
	{
		useSize += (int)(&buf[size - 1] - start);
		useSize += (int)(end - &buf[0]);
	}

	return useSize;
}

int RingBuffer::GetFreeSize()
{
	int freeSize = size - GetUseSize();
	return freeSize;
}

int RingBuffer::Enqueue(const char* chpData, int pSize)
{
	char* start = readPos;
	char* end = writePos;
	int inputSize = 0;

	// WritePos 경계면 이동
	if (end + pSize > &buf[size - 1])
	{
		pSize -= (int)(&buf[size - 1] - end);
		end = &buf[0] + pSize;
		
		
	}
	else
	{

	}

	return 1;
}

int RingBuffer::Dequeue(char* chpDest, int pSize)
{
	return 0;
}

int RingBuffer::Peek(char* chpDest, int pSize)
{
	return 0;
}

void RingBuffer::ClearBuffer()
{
	memset(buf, 0, sizeof(buf));
	writePos = &buf[0];
	readPos = &buf[0];
}