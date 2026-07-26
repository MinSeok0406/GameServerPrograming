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

private:
	char* buf;
	int totalSize;
	int size;
	char* writePos;
	char* readPos;

	RingBuffer(const RingBuffer& rb) = delete;
	RingBuffer& operator=(const RingBuffer& rb) = delete;
};