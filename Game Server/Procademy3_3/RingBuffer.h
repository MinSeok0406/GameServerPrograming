#pragma once
#include <Windows.h>

class RingBuffer
{
public:
	RingBuffer();
	RingBuffer(int pBufferSize);
	~RingBuffer();

	inline int		GetUseSize() { return size; }				// 현재 사용중인 용량 얻기.
	inline int		GetFreeSize() { return totalSize - size; }	// 현재 버퍼에 남은 용량 얻기.
	int				Enqueue(const char* chpData, int pSize);	// WritePos 에 데이타 넣음.
	int				Dequeue(char* chpDest, int pSize);			// ReadPos 에서 데이타 가져옴. ReadPos 이동.
	int				Peek(char* chpDest, int pSize);				// ReadPos 에서 데이타 읽어옴. ReadPos 고정.
	void			ClearBuffer();								// 버퍼의 모든 데이타 삭제.

	// 버퍼 포인터로 외부에서 한방에 읽고, 쓸 수 있는 길이.
	inline int DirectEnqueueSize() 
	{
		int freeSize = GetFreeSize();                       // size는 한 번만 읽기
		int toEnd = (int)(&buf[totalSize] - readPos);
		return (freeSize < toEnd) ? freeSize : toEnd;
	}					

	// 버퍼 포인터로 외부에서 한방에 읽고, 쓸 수 있는 길이.
	inline int DirectDequeueSize()
	{
		int useSize = GetUseSize();                         // size는 한 번만 읽기
		int toEnd = (int)(&buf[totalSize] - writePos);
		return (useSize < toEnd) ? useSize : toEnd;
	}

	int				MoveRear(int pSize);						// 원하는 길이만큼 읽기위치 에서 삭제 / 쓰기 위치 이동
	int				MoveFront(int pSize);						// 원하는 길이만큼 읽기위치 에서 삭제 / 쓰기 위치 이동

	inline char*	GetFrontBufferPtr() { return writePos; }	// 버퍼의 WritePos 포인터 얻음.
	inline char*	GetRearBufferPtr() { return readPos; }		// 버퍼의 RearPos 포인터 얻음.

	RingBuffer(const RingBuffer& rb) = delete;
	RingBuffer& operator=(const RingBuffer& rb) = delete;
	RingBuffer(RingBuffer&& rb) = delete;
	RingBuffer& operator=(RingBuffer&& rb) = delete;

private:
	char* buf;
	long totalSize;
	volatile long size;
	char* writePos;
	char* readPos;
};