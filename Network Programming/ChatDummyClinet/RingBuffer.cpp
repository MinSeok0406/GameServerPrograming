#include <cstdlib>
#include <cstring>
#include <algorithm>
#include "RingBuffer.h"

RingBuffer::RingBuffer() : buf(nullptr), size(0), totalSize(0), writePos(nullptr), readPos(nullptr)
{

}

RingBuffer::RingBuffer(uint32_t pBufferSize)
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

uint32_t RingBuffer::GetUseSize()
{
	return size;
}

uint32_t RingBuffer::GetFreeSize()
{
	return totalSize - size;
}

uint32_t RingBuffer::Enqueue(const char* chpData, uint32_t pSize)
{
	if (pSize > GetFreeSize())
	{
		return 0;
	}

	uint32_t remainSize = DirectEnqueueSize();
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

uint32_t RingBuffer::Dequeue(char* chpDest, uint32_t pSize)
{
	if (pSize > GetUseSize())
	{
		return 0;
	}

	uint32_t remainSize = DirectDequeueSize();
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

uint32_t RingBuffer::Peek(char* chpDest, uint32_t pSize)
{
	if (pSize > GetUseSize())
	{
		return 0;
	}

	uint32_t remainSize = DirectDequeueSize();
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

// 링 버퍼의 경계를 넘어갈 때, 두 값을 더해서 크기 자체는 맞지만 버퍼는 경계를 넘어서
// 이후의 값도 쓰거나 읽을 수 있다. 이 부분은 오류가 되기 때문에 고쳐야 함
uint32_t RingBuffer::DirectEnqueueSize()
{
	uint32_t enqueueSize = (uint32_t)(&buf[totalSize] - readPos);
	return std::min(GetFreeSize(), enqueueSize);
}

uint32_t RingBuffer::DirectDequeueSize()
{
	uint32_t dequeueSize = (uint32_t)(&buf[totalSize] - writePos);
	return std::min(GetUseSize(), dequeueSize);
}

uint32_t RingBuffer::MoveRear(uint32_t pSize)
{
	if (pSize > GetFreeSize())
	{
		return 0;
	}

	uint32_t commitSize = pSize;
	size += pSize;

	uint32_t moveSize = (uint32_t)(&buf[totalSize] - readPos);
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

uint32_t RingBuffer::MoveFront(uint32_t pSize)
{
	if (pSize > GetUseSize())
	{
		return 0;
	}

	uint32_t commitSize = pSize;
	size -= pSize;

	uint32_t moveSize = (uint32_t)(&buf[totalSize] - writePos);
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

char* RingBuffer::GetFrontBufferPtr()
{
	return writePos;
}

char* RingBuffer::GetRearBufferPtr()
{
	return readPos;
}