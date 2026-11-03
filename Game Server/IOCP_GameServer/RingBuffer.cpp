#include <cstdlib>
#include <cstring>
#include <algorithm>
#include "RingBuffer.h"

RingBuffer::RingBuffer() : buf(nullptr), size(0), totalSize(0), writePos(nullptr), readPos(nullptr)
{

}

RingBuffer::RingBuffer(int pBufferSize)
{
	buf = (char*)malloc(pBufferSize);
	totalSize = pBufferSize;
	size = 0;
	writePos = &buf[0];
	readPos = &buf[0];
}

RingBuffer::~RingBuffer()
{
	free(buf);
	writePos = nullptr;
	readPos = nullptr;
}

int RingBuffer::Enqueue(const char* chpData, int pSize)
{
	if (pSize > GetFreeSize())
	{
		return 0;
	}

	int remainSize = DirectEnqueueSize();
	// readPos 경계면 이동
	if (pSize <= remainSize)
	{
		memcpy(readPos, chpData, pSize);
	}
	else
	{
		memcpy(readPos, chpData, remainSize);
		memcpy(&buf[0], chpData + remainSize, pSize - remainSize);
	}

	MoveRear(pSize);

	return pSize;
}

int RingBuffer::Dequeue(char* chpDest, int pSize)
{
	if (pSize > GetUseSize())
	{
		return 0;
	}

	int remainSize = DirectDequeueSize();
	// writePos 경계면 이동
	if (pSize <= remainSize)
	{
		memcpy(chpDest, writePos, pSize);
	}
	else
	{
		memcpy(chpDest, writePos, remainSize);
		memcpy(chpDest + remainSize, &buf[0], pSize - remainSize);
	}

	MoveFront(pSize);

	return pSize;
}

int RingBuffer::Peek(char* chpDest, int pSize)
{
	if (pSize > GetUseSize())
	{
		return 0;
	}

	int remainSize = DirectDequeueSize();
	// writePos 경계면 이동
	if (pSize <= remainSize)
	{
		memcpy(chpDest, writePos, pSize);
	}
	else
	{
		memcpy(chpDest, writePos, remainSize);
		memcpy(chpDest + remainSize, &buf[0], pSize - remainSize);
	}

	return pSize;
}

void RingBuffer::ClearBuffer()
{
	memset(buf, 0, totalSize);
	size = 0;
	writePos = &buf[0];
	readPos = &buf[0];
}

int RingBuffer::MoveRear(int pSize)
{
	if (pSize > GetFreeSize())
	{
		return 0;
	}

	int commitSize = pSize;
	InterlockedExchangeAdd(&size, pSize);

	int moveSize = (int)(&buf[totalSize] - readPos);
	if (pSize >= moveSize)
	{
		readPos = &buf[0] + (pSize - moveSize);
	}
	else
	{
		readPos += pSize;
	}

	return commitSize;
}

int RingBuffer::MoveFront(int pSize)
{
	if (pSize > GetUseSize())
	{
		return 0;
	}

	int commitSize = pSize;
	InterlockedExchangeAdd(&size, -pSize);

	int moveSize = (int)(&buf[totalSize] - writePos);
	if (pSize >= moveSize)
	{
		writePos = &buf[0] + (pSize - moveSize);
	}
	else
	{
		writePos += pSize;
	}

	return commitSize;
}