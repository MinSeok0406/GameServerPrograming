#include "SerializationBuffer.h"
#include <cstdlib>
#include <cstring>

SerializationBuffer::SerializationBuffer()
{
	_buffersize = (unsigned int)PACKET::BUFFER_DEFAULT;
	_usesize = 0;
	_readpos = 0;
	_buffer = (char*)malloc(_buffersize);
}

SerializationBuffer::SerializationBuffer(int buffersize)
{
	_buffersize = buffersize;
	_usesize = 0;
	_readpos = 0;
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
	_readpos = 0;
}

int SerializationBuffer::moveWritePos(int size)
{
	if (getDataSize() < size)
	{
		return 0;
	}

	int decommitSize = size;
	_readpos += size;
	_usesize -= size;

	return decommitSize;
}

int SerializationBuffer::moveReadPos(int size)
{
	int remainSize = _buffersize - _usesize;
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

	return commitSize;
}

int SerializationBuffer::getData(char* dest, int size)
{
	if (getDataSize() < size)
	{
		return 0;
	}

	memcpy(dest, _buffer + _readpos, size);
	moveWritePos(size);

	return size;
}

int SerializationBuffer::putData(char* src, int size)
{
	int remainSize = _buffersize - _usesize;
	if (size >= remainSize)
	{
		_buffersize *= 2;
		int* reallocRet = (int*)realloc(_buffer, _buffersize);
		if (reallocRet == nullptr)
		{
			// 실패 로그
		}
	}

	memcpy(_buffer + _readpos + _usesize, src, size);
	moveReadPos(size);

	return 0;
}

SerializationBuffer& SerializationBuffer::operator=(SerializationBuffer& packet)
{
	if (this == &packet)
	{
		return *this;
	}

	this->_buffersize = packet._buffersize;
	this->_usesize = packet._usesize;
	free(this->_buffer);
	this->_buffer = (char*)malloc(_buffersize);
	memcpy(this->_buffer, packet._buffer, _buffersize);
	return *this;
}

SerializationBuffer& SerializationBuffer::operator<<(unsigned char value)
{
	this->putData((char*)&value, sizeof(value));
	return *this;
}

SerializationBuffer& SerializationBuffer::operator<<(char value)
{
	this->putData((char*)&value, sizeof(value));
	return *this;
}

SerializationBuffer& SerializationBuffer::operator<<(unsigned short value)
{
	this->putData((char*)&value, sizeof(value));
	return *this;
}

SerializationBuffer& SerializationBuffer::operator<<(short value)
{
	this->putData((char*)&value, sizeof(value));
	return *this;
}

SerializationBuffer& SerializationBuffer::operator<<(unsigned int value)
{
	this->putData((char*)&value, sizeof(value));
	return *this;
}

SerializationBuffer& SerializationBuffer::operator<<(int value)
{
	this->putData((char*)&value, sizeof(value));
	return *this;
}

SerializationBuffer& SerializationBuffer::operator<<(unsigned long value)
{
	this->putData((char*)&value, sizeof(value));
	return *this;
}

SerializationBuffer& SerializationBuffer::operator<<(long value)
{
	this->putData((char*)&value, sizeof(value));
	return *this;
}

SerializationBuffer& SerializationBuffer::operator<<(unsigned long long value)
{
	this->putData((char*)&value, sizeof(value));
	return *this;
}

SerializationBuffer& SerializationBuffer::operator<<(long long value)
{
	this->putData((char*)&value, sizeof(value));
	return *this;
}

SerializationBuffer& SerializationBuffer::operator<<(float value)
{
	this->putData((char*)&value, sizeof(value));
	return *this;
}

SerializationBuffer& SerializationBuffer::operator<<(double value)
{
	this->putData((char*)&value, sizeof(value));
	return *this;
}

SerializationBuffer& SerializationBuffer::operator>>(unsigned char& value)
{
	this->getData((char*)&value, sizeof(value));
	return *this;
}

SerializationBuffer& SerializationBuffer::operator>>(char& value)
{
	this->getData((char*)&value, sizeof(value));
	return *this;
}

SerializationBuffer& SerializationBuffer::operator>>(unsigned short& value)
{
	this->getData((char*)&value, sizeof(value));
	return *this;
}

SerializationBuffer& SerializationBuffer::operator>>(short& value)
{
	this->getData((char*)&value, sizeof(value));
	return *this;
}

SerializationBuffer& SerializationBuffer::operator>>(unsigned int& value)
{
	this->getData((char*)&value, sizeof(value));
	return *this;
}

SerializationBuffer& SerializationBuffer::operator>>(int& value)
{
	this->getData((char*)&value, sizeof(value));
	return *this;
}

SerializationBuffer& SerializationBuffer::operator>>(unsigned long& value)
{
	this->getData((char*)&value, sizeof(value));
	return *this;
}

SerializationBuffer& SerializationBuffer::operator>>(long& value)
{
	this->getData((char*)&value, sizeof(value));
	return *this;
}

SerializationBuffer& SerializationBuffer::operator>>(unsigned long long& value)
{
	this->getData((char*)&value, sizeof(value));
	return *this;
}

SerializationBuffer& SerializationBuffer::operator>>(long long& value)
{
	this->getData((char*)&value, sizeof(value));
	return *this;
}

SerializationBuffer& SerializationBuffer::operator>>(float& value)
{
	this->getData((char*)&value, sizeof(value));
	return *this;
}

SerializationBuffer& SerializationBuffer::operator>>(double& value)
{
	this->getData((char*)&value, sizeof(value));
	return *this;
}