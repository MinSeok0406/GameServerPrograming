#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <vector>
#include <ctime>
using ll = long long;

class MetricLogger
{
public:
	// 저장할 시간 입력(단위 : 초)
	MetricLogger(double saveSec = 3600.0)
		: _saveSec(saveSec)
	{

	}

	// 로깅 초기화
	void Init()
	{
		QueryPerformanceFrequency(&_freq);
		QueryPerformanceCounter(&_startTime);
		_lastCheckTime = _startTime;
		_lastSaveTime = _startTime;
		_lastBufferTime = _startTime;

		time_t t = time(nullptr);
		tm tmInfo;
		localtime_s(&tmInfo, &t);
		sprintf(_fileName, "metrics_%04d%02d%02d_%02d%02d%02d.csv",
			tmInfo.tm_year + 1900, tmInfo.tm_mon + 1, tmInfo.tm_mday,
			tmInfo.tm_hour, tmInfo.tm_min, tmInfo.tm_sec);
	}

	// 한 프레임 당 한 틱 증가 -> tps 측정 목적
	void OnTick() { ++_tickCount; ++_tickCountMinute; }

	// 클라이언트 accept 함수 횟수 측정
	void OnTickAccept() { ++_tickAcceptCount; ++_tickAcceptCountMinute; }

	// 클라이언트 accept 함수 성공 횟수 측정 
	void OnTickSuccess() { ++_tickSuccessCount; ++_tickSuccessCountMinute; }

	// 완성된 패킷 수신 횟수
	void OnPacketRecv() { ++_recvCount; ++_recvCountMinute; }

	// 완성된 패킷 송신 횟수
	void OnPacketSend() { ++_sendCount; ++_sendCountMinute; }

	// 완성되지 않고 패기된 패킷 횟수
	void OnPacketSendDrop() { ++_sendDropCount; ++_sendDropCountMinute; }

	void Update()
	{
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);

		double sinceCheckSec = Elapsed(_lastCheckTime, now);
		if (sinceCheckSec >= 1.0)
		{
			Record(now, sinceCheckSec);
			_lastCheckTime = now;
		}

		double sinceBufferSec = Elapsed(_lastBufferTime, now);
		if (sinceBufferSec >= 60.0)
		{
			RecordToBuffer(now, sinceBufferSec);   // 1분마다 buffer에 push_back
			_lastBufferTime = now;
		}

		double sinceSaveSec = Elapsed(_lastSaveTime, now);
		if (sinceSaveSec >= _saveSec)
		{
			SaveToFile();
			_lastSaveTime = now;
		}
	}

private:
	struct Metric
	{
		double _elapsedSec;		// 측정 시간
		double _tps;			// tps 측정
		double _tpsAccept;		// accept 횟수(클라 접속 빈도 수) 측정
		double _tpsSuccess;		// accept 성공 횟수 측정
		double _recvPerSec;		// 1초당 recv 횟수
		double _sendPerSec;		// 1초당 send 횟수
		double _sendDropPerSec;	// 1초당 send Drop 횟수
	};

	// 시간 구하기
	double Elapsed(LARGE_INTEGER from, LARGE_INTEGER to) const
	{
		return (double)(to.QuadPart - from.QuadPart) / _freq.QuadPart;
	}

	// 파일에 저장할 지표 작성
	void Record(LARGE_INTEGER now, double intervalSec)
	{
		double tps = _tickCount / intervalSec;
		double tpsAccept = _tickAcceptCount / intervalSec;
		double tpsSuccess = _tickSuccessCount / intervalSec;
		double recvPerSec = _recvCount / intervalSec;
		double sendPerSec = _sendCount / intervalSec;
		double sendDropPerSec = _sendDropCount / intervalSec;

		printf("[TPS: %.3f] [TPS Accept: %.3f] [TPS Success: %.3f] [Recv/sec: %.3f] [Send/sec: %.3f] [SendDrop/sec : %.3f]\n",
			tps, tpsAccept, tpsSuccess, recvPerSec, sendPerSec, sendDropPerSec);

		_tickCount = 0;
		_tickAcceptCount = 0;
		_tickSuccessCount = 0;
		_recvCount = 0;
		_sendCount = 0;
		_sendDropCount = 0;
	}

	void RecordToBuffer(LARGE_INTEGER now, double intervalSec)
	{
		double tps = _tickCountMinute / intervalSec;
		double tpsAccept = _tickAcceptCountMinute / intervalSec;
		double tpsSuccess = _tickSuccessCountMinute / intervalSec;
		double recvPerSec = _recvCountMinute / intervalSec;
		double sendPerSec = _sendCountMinute / intervalSec;
		double sendDropPerSec = _sendDropCountMinute / intervalSec;

		double totalElapsedSec = Elapsed(_startTime, now);
		_buffer.push_back({ totalElapsedSec, tps, tpsAccept, tpsSuccess, recvPerSec, sendPerSec, sendDropPerSec });

		_tickCountMinute = 0;
		_tickAcceptCountMinute = 0;
		_tickSuccessCountMinute = 0;
		_recvCountMinute = 0;
		_sendCountMinute = 0;
		_sendDropCountMinute = 0;
	}

	// 지표가 담긴 파일 생성 후 저장
	void SaveToFile()
	{
		if (_buffer.empty())
		{
			return;
		}

		FILE* fp;
		fopen_s(&fp, _fileName, "a");
		if (fp == nullptr)
		{
			printf("%s open fail\n", _fileName);
			return;
		}

		fseek(fp, 0, SEEK_END);
		if (ftell(fp) == 0)
		{
			fprintf(fp, "ElapsedSec,TPS,TPSAccept,TPSSuccess,RecvPerSec,SendPerSec,SendDropSec\n");
		}

		for (auto& sample : _buffer)
		{
			fprintf(fp, "Total Time : %.3f \t TPS : %.3f \t TPS_Accept : %.3f \t TPS_Success : %.3f \t RecvSec : %.3f \t SendSec : %.3f \t SendDropSec : %.3f\n",
				sample._elapsedSec, sample._tps, sample._tpsAccept, sample._tpsSuccess, sample._recvPerSec, sample._sendPerSec, sample._sendDropPerSec);
		}

		fclose(fp);
		_buffer.clear();
	}

	// 정밀한 시간 측정 변수
	LARGE_INTEGER _freq {};
	LARGE_INTEGER _startTime {};

	// 1초당 시간 측정 / 사용자가 정한 시간마다 파일 저장 시간
	LARGE_INTEGER _lastCheckTime {};
	LARGE_INTEGER _lastSaveTime {};

	// 초당 출력 카운트
	ll _tickCount = 0;
	ll _tickAcceptCount = 0;
	ll _tickSuccessCount = 0;
	ll _recvCount = 0;
	ll _sendCount = 0;
	ll _sendDropCount = 0;

	// 분당 저장 카운트
	ll _tickCountMinute = 0;
	ll _tickAcceptCountMinute = 0;
	ll _tickSuccessCountMinute = 0;
	ll _recvCountMinute = 0;
	ll _sendCountMinute = 0;
	ll _sendDropCountMinute = 0;

	LARGE_INTEGER _lastBufferTime{};

	double _saveSec;
	std::vector<Metric> _buffer;
	char _fileName[64] = {};
};