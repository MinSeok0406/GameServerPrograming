#include <cstdlib>
#include <cstring>
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
		pSize -= (int)(&buf[totalSize] - writePos);
		while (writePos <= &buf[totalSize - 1])
		{
			*writePos = *chpData;
			writePos++;
			chpData++;
		}
		writePos = &buf[0];

		while (pSize > 0)
		{
			*writePos = *chpData;
			writePos++;
			chpData++;
			pSize--;
		}
	}
	else
	{
		while (pSize > 0)
		{
			*writePos = *chpData;
			writePos++;
			chpData++;
			pSize--;
		}
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
		pSize -= (int)(&buf[totalSize] - readPos);
		while (readPos <= &buf[totalSize - 1])
		{
			*chpDest = *readPos;
			readPos++;
			chpDest++;
		}
		readPos = &buf[0];

		while (pSize > 0)
		{
			*chpDest = *readPos;
			readPos++;
			chpDest++;
			pSize--;
		}
	}
	else
	{
		while (pSize > 0)
		{
			*chpDest = *readPos;
			readPos++;
			chpDest++;
			pSize--;
		}
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
		pSize -= (int)(&buf[totalSize] - read);
		while (read <= &buf[totalSize - 1])
		{
			*dest = *read;
			read++;
			dest++;
		}
		read = &buf[0];

		while (pSize > 0)
		{
			*dest = *read;
			read++;
			dest++;
			pSize--;
		}
	}
	else
	{
		while (pSize > 0)
		{
			*dest = *read;
			read++;
			dest++;
			pSize--;
		}
	}

	return outputSize;
}

void RingBuffer::ClearBuffer()
{
	memset(buf, 0, totalSize);
	writePos = &buf[0];
	readPos = &buf[0];
}