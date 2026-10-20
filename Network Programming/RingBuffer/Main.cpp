#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <process.h>
#include <Windows.h>
#include <conio.h>
#include <random>
#include <list>
#include "RingBuffer.h"
using namespace std;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

HANDLE g_hEvent;
RingBuffer g_rb(3000);

const char			g_pattern[] = "1234567890 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ!";
const char			g_patternLen = sizeof(g_pattern) - 1;
const uint64_t		TEST_DURATION_MS = 24ULL * 60 * 60 * 1000;  // 24시간

volatile bool		g_stop = false;			// main → 종료 요청
volatile bool		g_writerDone = false;  // writer → 더 이상 안 넣음
volatile uint64_t	g_writeTotal = 0;      // writer만 씀
volatile uint64_t	g_readTotal = 0;      // reader만 씀

void FillPattern(char* dest, int len, int& patternPos)
{
	for (int i = 0; i < len; ++i)
	{
		dest[i] = g_pattern[patternPos];
		if (++patternPos == g_patternLen)
			patternPos = 0;
	}
}

void CheckPattern(const char* data, int len, int& patternPos, uint64_t offset)
{
	for (int i = 0; i < len; ++i)
	{
		if (data[i] != g_pattern[patternPos])
		{
			printf("offset=%llu\n", offset + i);
			__debugbreak();
		}

		if (++patternPos == g_patternLen) 
			patternPos = 0;
	}
}

uint32_t WINAPI readProc(LPVOID param)
{
	WaitForSingleObject(g_hEvent, INFINITE);
	mt19937 rng(GetCurrentThreadId());
	char temp[200];
	int patternPos = 0;
	uint64_t readTotal = 0;

	while (true)
	{
		int useSize = g_rb.GetUseSize();
		if (useSize == 0)
		{
			if (g_writerDone && g_rb.GetUseSize() == 0)
				break;

			_mm_pause();
			continue;
		}

		int mode = rng() % 3;
		int len = (int)(rng() % 200) + 1;
		if (len > useSize)
			len = useSize;

		if (mode == 0)
		{
			g_rb.Dequeue(temp, len);
			CheckPattern(temp, len, patternPos, readTotal);
		}
		else if (mode == 1)
		{
			g_rb.Peek(temp, len);
			CheckPattern(temp, len, patternPos, readTotal);
			g_rb.MoveFront(len);
		}
		else if (mode == 2)
		{
			int direct = g_rb.DirectDequeueSize();
			if (len > direct) 
				len = direct;
			CheckPattern(g_rb.GetFrontBufferPtr(), len, patternPos, readTotal);
			g_rb.MoveFront(len);
		}

		readTotal += len;
		g_readTotal = readTotal;
	}

	return 0;
}

uint32_t WINAPI writeProc(LPVOID param)
{
	WaitForSingleObject(g_hEvent, INFINITE);
	mt19937 rng(GetCurrentThreadId());
	char temp[200];
	int patternPos = 0;
	uint64_t writeTotal = 0;

	while (!g_stop)
	{
		int freeSize = g_rb.GetFreeSize();
		if (freeSize == 0)
		{
			_mm_pause();
			continue;
		}

		int len = (int)(rng() % 200) + 1;
		if (len > freeSize)
			len = freeSize;

		if (rng() % 2 == 0)
		{
			FillPattern(temp, len, patternPos);
			g_rb.Enqueue(temp, len);
		}
		else
		{
			int direct = g_rb.DirectEnqueueSize();
			if (len > direct)
				len = direct;
			FillPattern(g_rb.GetRearBufferPtr(), len, patternPos);
			g_rb.MoveRear(len);
		}

		writeTotal += len;
		g_writeTotal = writeTotal;
	}

	g_writerDone = true;
	return 0;
}

int wmain()
{
	HANDLE hThread[2];

	g_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, readProc, NULL, NULL, NULL);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, writeProc, NULL, NULL, NULL);
	
	printf("테스트 시작 ('q' 키로 종료)\n");
	uint64_t startTick = GetTickCount64();
	SetEvent(g_hEvent);

	while (!g_stop)
	{
		Sleep(100);
		if (GetTickCount64() - startTick >= TEST_DURATION_MS)
			g_stop = true;
		if (_kbhit() && _getch() == 'q')
			g_stop = true;
	}

	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

	if (g_readTotal == g_writeTotal)
	{
		printf("[PASS] %llu bytes 검증 완료\n", g_readTotal);
	}
	else
	{
		printf("[FAIL] 쓴 양=%llu, 읽은 양=%llu\n", g_writeTotal, g_readTotal);
	}

	for (auto i = 0; i < 2; ++i)
	{
		CloseHandle(hThread[i]);
	}

	CloseHandle(g_hEvent);

	return 0;
}