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

int RingBuffer::GetUseSize()
{
	return size;
}

int RingBuffer::GetFreeSize()
{
	return totalSize - size;
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

// 링 버퍼의 경계를 넘어갈 때, 두 값을 더해서 크기 자체는 맞지만 버퍼는 경계를 넘어서
// 이후의 값도 쓰거나 읽을 수 있다. 이 부분은 오류가 되기 때문에 고쳐야 함
int RingBuffer::DirectEnqueueSize()
{
	int enqueueSize = (int)(&buf[totalSize] - readPos);
	return std::min(GetFreeSize(), enqueueSize);
}

int RingBuffer::DirectDequeueSize()
{
	int dequeueSize = (int)(&buf[totalSize] - writePos);
	return std::min(GetUseSize(), dequeueSize);
}

int RingBuffer::MoveRear(int pSize)
{
	if (pSize > GetFreeSize())
	{
		return 0;
	}

	int commitSize = pSize;
	size += pSize;

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
	size -= pSize;

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

char* RingBuffer::GetFrontBufferPtr()
{
	return writePos;
}

char* RingBuffer::GetRearBufferPtr()
{
	return readPos;
}