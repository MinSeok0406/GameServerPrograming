#pragma once
class SerializationBuffer
{
public:
	enum class PACKET
	{
		BUFFER_DEFAULT = 1400									// 패킷의 기본 버퍼 사이즈.
	};

	SerializationBuffer();
	SerializationBuffer(int buffersize);
	virtual ~SerializationBuffer();

	void clear();												// 패킷 청소.
	int getBufferSize() { return _buffersize; }					// 버퍼 사이즈 얻기.
	int getDataSize() { return _usesize; }						// 현재 사용중인 사이즈 얻기.

	char* getBufferPtr() { return _buffer; }					// 버퍼 포인터 얻기.
	int moveWritePos(int size);									// 버퍼 Pos 감소.
	int moveReadPos(int size);									// 버퍼 Pos 증가.

	int getData(char* dest, int size);							// 데이타 얻기.
	int putData(char* src, int size);							// 데이타 삽입. (배열, 구조체등 넣기위한 용도)

	// 연산자 오버로딩 버전
	SerializationBuffer& operator=(SerializationBuffer& packet);

	// 데이터 넣기.
	SerializationBuffer& operator<<(unsigned char value);
	SerializationBuffer& operator<<(char value);
	SerializationBuffer& operator<<(unsigned short value);
	SerializationBuffer& operator<<(short value);
	SerializationBuffer& operator<<(unsigned int value);
	SerializationBuffer& operator<<(int value);
	SerializationBuffer& operator<<(unsigned long value);
	SerializationBuffer& operator<<(long value);
	SerializationBuffer& operator<<(unsigned long long value);
	SerializationBuffer& operator<<(long long value);
	SerializationBuffer& operator<<(float value);
	SerializationBuffer& operator<<(double value);

	// 데이터 빼기.
	SerializationBuffer& operator>>(unsigned char& value);
	SerializationBuffer& operator>>(char& value);
	SerializationBuffer& operator>>(unsigned short& value);
	SerializationBuffer& operator>>(short& value);
	SerializationBuffer& operator>>(unsigned int& value);
	SerializationBuffer& operator>>(int& value);
	SerializationBuffer& operator>>(unsigned long& value);
	SerializationBuffer& operator>>(long& value);
	SerializationBuffer& operator>>(unsigned long long& value);
	SerializationBuffer& operator>>(long long& value);
	SerializationBuffer& operator>>(float& value);
	SerializationBuffer& operator>>(double& value);

private:
	unsigned int	_buffersize;
	unsigned int	_usesize;
	unsigned int	_readpos;
	char*			_buffer;
};