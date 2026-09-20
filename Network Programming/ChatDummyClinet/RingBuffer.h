#pragma once

class RingBuffer
{
public:
	RingBuffer();
	RingBuffer(uint32_t pBufferSize);
	~RingBuffer();

	uint32_t		GetUseSize();										// 현재 사용중인 용량 얻기.
	uint32_t		GetFreeSize();										// 현재 버퍼에 남은 용량 얻기.
	uint32_t		Enqueue(const char* chpData, uint32_t pSize);		// WritePos 에 데이타 넣음.
	uint32_t		Dequeue(char* chpDest, uint32_t pSize);				// ReadPos 에서 데이타 가져옴. ReadPos 이동.
	uint32_t		Peek(char* chpDest, uint32_t pSize);				// ReadPos 에서 데이타 읽어옴. ReadPos 고정.
	void			ClearBuffer();										// 버퍼의 모든 데이타 삭제.

	uint32_t		DirectEnqueueSize();							// 버퍼 포인터로 외부에서 한방에 읽고, 쓸 수 있는 길이.
	uint32_t		DirectDequeueSize();							// 버퍼 포인터로 외부에서 한방에 읽고, 쓸 수 있는 길이.
	uint32_t		MoveRear(uint32_t pSize);						// 원하는 길이만큼 읽기위치 에서 삭제 / 쓰기 위치 이동
	uint32_t		MoveFront(uint32_t pSize);						// 원하는 길이만큼 읽기위치 에서 삭제 / 쓰기 위치 이동
	char*			GetFrontBufferPtr();							// 버퍼의 WritePos 포인터 얻음.
	char*			GetRearBufferPtr();								// 버퍼의 RearPos 포인터 얻음.

	RingBuffer(const RingBuffer& rb) = default;
	RingBuffer& operator=(const RingBuffer& rb) = default;
	RingBuffer(RingBuffer&& rb) = default;
	RingBuffer& operator=(RingBuffer&& rb) = default;

private:
	char*		buf;
	uint32_t	totalSize;
	uint32_t	size;
	char*		writePos;
	char*		readPos;
};