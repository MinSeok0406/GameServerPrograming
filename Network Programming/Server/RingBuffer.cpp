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
	int inputSize = pSize;

	if (pSize > GetFreeSize())
	{
		return 0;
	}

	size += pSize;

	// WritePos 경계면 이동
	if (writePos + pSize > &buf[totalSize])
	{
		int offset = (int)(&buf[totalSize] - writePos);
		pSize -= offset;
		memcpy(writePos, chpData, offset);
		chpData += offset;
		writePos = &buf[0];

		memcpy(writePos, chpData, pSize);
		writePos += pSize;
		*writePos = '\0';
	}
	else
	{
		memcpy(writePos, chpData, pSize);
		writePos += pSize;
		*writePos = '\0';
	}

	return inputSize;
}

int RingBuffer::Dequeue(char* chpDest, int pSize)
{
	int outputSize = pSize;

	if (pSize > GetUseSize())
	{
		return 0;
	}

	size -= pSize;

	// readPos 경계면 이동
	if (readPos + pSize > &buf[totalSize])
	{
		int offset = (int)(&buf[totalSize] - readPos);
		pSize -= offset;
		memcpy(chpDest, readPos, offset);
		chpDest += offset;
		readPos = &buf[0];

		memcpy(chpDest, readPos, pSize);
		chpDest += pSize;
		readPos += pSize;
		*readPos = '\0';
	}
	else
	{
		memcpy(chpDest, readPos, pSize);
		chpDest += pSize;
		readPos += pSize;
		*readPos = '\0';
	}

	return outputSize;
}

int RingBuffer::Peek(char* chpDest, int pSize)
{
	char* read = readPos;
	char* dest = chpDest;
	int outputSize = pSize;

	if (pSize > GetUseSize())
	{
		return 0;
	}

	// readPos 경계면 넘어감
	if (read + pSize > &buf[totalSize])
	{
		int offset = (int)(&buf[totalSize] - read);
		pSize -= offset;
		memcpy(dest, read, offset);
		dest += offset;
		read = &buf[0];

		memcpy(dest, read, pSize);
		dest += pSize;
		read += pSize;
	}
	else
	{
		memcpy(dest, read, pSize);
		dest += pSize;
		read += pSize;
	}

	return outputSize;
}

void RingBuffer::ClearBuffer()
{
	memset(buf, 0, totalSize);
	size = 0;
	writePos = &buf[0];
	readPos = &buf[0];
}

int RingBuffer::DirectEnqueueSize()
{
	return std::min(GetFreeSize(), (int)(&buf[totalSize] - writePos));
}

int RingBuffer::DirectDequeueSize()
{
	return std::min(GetUseSize(), (int)(&buf[totalSize] - readPos));
}

int RingBuffer::MoveRear(int pSize)
{
	if (pSize > GetFreeSize())
	{
		return 0;
	}

	size += pSize;

	if (writePos + pSize > &buf[totalSize])
	{
		int offset = (int)(&buf[totalSize] - writePos);
		pSize -= offset;
		writePos = &buf[0];
		writePos += pSize;
	}
	else
	{
		writePos += pSize;
	}

	return pSize;
}

int RingBuffer::MoveFront(int pSize)
{
	if (pSize > GetUseSize())
	{
		return 0;
	}

	size -= pSize;

	if (readPos + pSize > &buf[totalSize])
	{
		int offset = (int)(&buf[totalSize] - readPos);
		pSize -= offset;
		readPos = &buf[0];
		readPos += pSize;
	}
	else
	{
		readPos += pSize;
	}

	return pSize;
}

char* RingBuffer::GetFrontBufferPtr()
{
	return readPos;
}

char* RingBuffer::GetRearBufferPtr()
{
	return writePos;
}