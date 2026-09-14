#pragma once

class RingBuffer
{
public:
	RingBuffer();
	RingBuffer(int pBufferSize);
	~RingBuffer();

	int		GetUseSize();								// 현재 사용중인 용량 얻기.
	int		GetFreeSize();								// 현재 버퍼에 남은 용량 얻기.
	int		Enqueue(const char* chpData, int pSize);	// WritePos 에 데이타 넣음.
	int		Dequeue(char* chpDest, int pSize);			// ReadPos 에서 데이타 가져옴. ReadPos 이동.
	int		Peek(char* chpDest, int pSize);				// ReadPos 에서 데이타 읽어옴. ReadPos 고정.
	void	ClearBuffer();								// 버퍼의 모든 데이타 삭제.

	int		DirectEnqueueSize();						// 버퍼 포인터로 외부에서 한방에 읽고, 쓸 수 있는 길이.
	int		DirectDequeueSize();						// 버퍼 포인터로 외부에서 한방에 읽고, 쓸 수 있는 길이.
	int		MoveRear(int pSize);						// 원하는 길이만큼 읽기위치 에서 삭제 / 쓰기 위치 이동
	int		MoveFront(int pSize);						// 원하는 길이만큼 읽기위치 에서 삭제 / 쓰기 위치 이동
	char*	GetFrontBufferPtr();						// 버퍼의 WritePos 포인터 얻음.
	char*	GetRearBufferPtr();							// 버퍼의 RearPos 포인터 얻음.

	RingBuffer(const RingBuffer& rb) = default;
	RingBuffer& operator=(const RingBuffer& rb) = default;
	RingBuffer(RingBuffer&& rb) = default;
	RingBuffer& operator=(RingBuffer&& rb) = default;

private:
	char* buf;
	int totalSize;
	int size;
	char* writePos;
	char* readPos;
};