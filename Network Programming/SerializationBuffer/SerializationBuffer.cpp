#include "SerializationBuffer.h"
#include <cstdlib>
#include <cstring>

SerializationBuffer::SerializationBuffer()
{
	_buffersize = (unsigned int)PACKET::BUFFER_DEFAULT;
	_usesize = 0;
	_buffer = (char*)malloc(_buffersize);
}

SerializationBuffer::SerializationBuffer(int buffersize)
{
	_buffersize = buffersize;
	_usesize = 0;
	_buffer = (char*)malloc(_buffersize);
}

SerializationBuffer::~SerializationBuffer()
{
	free(_buffer);
}

void SerializationBuffer::clear()
{
	free(_buffer);
	_buffer = (char*)malloc(_buffersize);
	_usesize = 0;
}

int SerializationBuffer::moveWritePos(int size)
{
	if (getDataSize() < size)
	{
		return 0;
	}

	int decommitSize = size;
	_usesize -= size;
	_buffer -= size;

	return decommitSize;
}

int SerializationBuffer::moveReadPos(int size)
{
	int remainSize = (int)(&_buffer[_buffersize] - _buffer);
	if (size >= remainSize)
	{
		_buffersize *= 2;
		int* reallocRet = (int*)realloc(_buffer, _buffersize);
		if (reallocRet == nullptr)
		{
			// 실패 로그
		}
	}

	int commitSize = size;
	_usesize += size;
	_buffer += size;

	return commitSize;
}

int SerializationBuffer::getData(char* dest, int size)
{
	if (getDataSize() < size)
	{
		return 0;
	}

	memcpy(dest, _buffer, size);
	moveWritePos(size);

	return size;
}

int SerializationBuffer::putData(char* src, int size)
{
	int remainSize = (int)(&_buffer[_buffersize] - _buffer);
	if (size >= remainSize)
	{
		_buffersize *= 2;
		int* reallocRet = (int*)realloc(_buffer, _buffersize);
		if (reallocRet == nullptr)
		{
			// 실패 로그
		}
	}

	memcpy(_buffer, src, size);
	moveReadPos(size);

	return 0;
}

SerializationBuffer& SerializationBuffer::operator=(SerializationBuffer& packet)
{
	this->_buffersize = packet._buffersize;
	this->_usesize = packet._usesize;
	free(this->_buffer);
	this->_buffer = (char*)malloc(_buffersize);
	memcpy(this->_buffer, packet._buffer, _buffersize);
	return *this;
}

SerializationBuffer& SerializationBuffer::operator<<(unsigned char value)
{
	
}

SerializationBuffer& SerializationBuffer::operator<<(char value)
{
	
}

SerializationBuffer& SerializationBuffer::operator<<(unsigned short value)
{
	
}

SerializationBuffer& SerializationBuffer::operator<<(short value)
{
	
}

SerializationBuffer& SerializationBuffer::operator<<(unsigned int value)
{
	
}

SerializationBuffer& SerializationBuffer::operator<<(int value)
{
	
}

SerializationBuffer& SerializationBuffer::operator<<(unsigned long value)
{
	
}

SerializationBuffer& SerializationBuffer::operator<<(long value)
{
	
}

SerializationBuffer& SerializationBuffer::operator<<(unsigned long long value)
{
	
}

SerializationBuffer& SerializationBuffer::operator<<(long long value)
{
	
}

SerializationBuffer& SerializationBuffer::operator<<(float value)
{
	
}

SerializationBuffer& SerializationBuffer::operator<<(double value)
{
	
}

SerializationBuffer& SerializationBuffer::operator>>(unsigned char& value)
{
	
}

SerializationBuffer& SerializationBuffer::operator>>(char& value)
{
	
}

SerializationBuffer& SerializationBuffer::operator>>(unsigned short& value)
{
	
}

SerializationBuffer& SerializationBuffer::operator>>(short& value)
{
	
}

SerializationBuffer& SerializationBuffer::operator>>(unsigned int& value)
{
	
}

SerializationBuffer& SerializationBuffer::operator>>(int& value)
{
	
}

SerializationBuffer& SerializationBuffer::operator>>(unsigned long& value)
{
	
}

SerializationBuffer& SerializationBuffer::operator>>(long& value)
{
	
}

SerializationBuffer& SerializationBuffer::operator>>(unsigned long long& value)
{
	
}

SerializationBuffer& SerializationBuffer::operator>>(long long& value)
{
	
}

SerializationBuffer& SerializationBuffer::operator>>(float& value)
{
	
}

SerializationBuffer& SerializationBuffer::operator>>(double& value)
{
	
}