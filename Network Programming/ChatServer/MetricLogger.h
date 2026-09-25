#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <cstdio>
#include <ctime>
#include <vector>
#include <limits>
using ll = long long;

struct MinAvgMax
{
	double _min = (std::numeric_limits<double>::max)();
	double _max = (std::numeric_limits<double>::lowest)();
	double _sum = 0.0;
	ll _count = 0;

	void Add(double v)
	{
		if (v < _min) _min = v;
		if (v > _max) _max = v;
		_sum += v;
		++_count;
	}

	double Min() const { return _count ? _min : 0.0; }
	double Avg() const { return _count ? _sum / (double)_count : 0.0; }
	double Max() const { return _count ? _max : 0.0; }
	ll Count() const { return _count; }
	void Reset() { *this = MinAvgMax(); }
};

//  [프레임(틱) 처리 시간]
//    - 매 프레임마다 측정 → 1분 구간의 min / avg / max (ms)
//    - select 대기 시간은 제외 (BeginWait/EndWait 사이 시간을 뺌)
//
//  [패킷 / TPS 지표]
//    - 1초마다 "초당 횟수"를 샘플 1개로 만들고 → 1분 구간(샘플 60개)의 min / avg / max
//    - 프레임 단위로 패킷 수를 재면 대부분 0이라 min이 의미가 없어서 1초 샘플 단위로 집계
class MetricLogger
{
public:
	//  windowSec : min/avg/max를 묶어서 CSV 한 줄로 만드는 구간(초)
	//  saveSec   : 모아둔 줄을 파일에 쓰는 주기(초)
	MetricLogger(double windowSec = 60.0, double saveSec = 60.0)
		: _windowSec(windowSec), _saveSec(saveSec)
	{}

	~MetricLogger()
	{
		Flush();
	}

	// 로깅 초기화
	void Init()
	{
		QueryPerformanceFrequency(&_freq);
		QueryPerformanceCounter(&_startTime);
		_lastCheckTime = _startTime;
		_lastWindowTime = _startTime;
		_lastSaveTime = _startTime;

		time_t t = time(nullptr);
		tm tmInfo;
		localtime_s(&tmInfo, &t);
		sprintf_s(_fileName, _countof(_fileName), "metrics_%04d%02d%02d_%02d%02d%02d.csv",
			tmInfo.tm_year + 1900, tmInfo.tm_mon + 1, tmInfo.tm_mday,
			tmInfo.tm_hour, tmInfo.tm_min, tmInfo.tm_sec);

		_initialized = true;
	}

	// ======================== 프레임 계측 함수 ========================
	// 루프 맨 위에서 호출
	void BeginFrame()
	{
		QueryPerformanceCounter(&_frameStart);
		_waitTicks = 0;
		_inFrame = true;
	}

	// select 직전 / 직후에 호출 → 이 구간은 처리 시간에서 제외
	void BeginWait() { QueryPerformanceCounter(&_waitStart); }
	void EndWait()
	{
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		_waitTicks += now.QuadPart - _waitStart.QuadPart;
	}

	// 프레임 처리가 끝난 뒤 호출 (파일 I/O가 있는 Update() 전에)
	void EndFrame()
	{
		if (!_inFrame)
		{
			return;
		}
		_inFrame = false;

		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		ll workTicks = (now.QuadPart - _frameStart.QuadPart) - _waitTicks;
		double ms = (double)workTicks * 1000.0 / (double)_freq.QuadPart;

		_frameMsWindow.Add(ms);
		_frameMsSecond.Add(ms);
		++_tickCount;
	}
	// ============================================================

	// ======================== 카운터 함수 ========================
	// 클라이언트 accept 함수 호출 횟수
	void OnTickAccept() { ++_acceptCount; }

	// 클라이언트 accept 성공 횟수
	void OnTickSuccess() { ++_acceptSuccessCount; }

	// 완성된 패킷 수신 횟수
	void OnPacketRecv() { ++_recvCount; }

	// 송신 큐에 넣은 패킷 수 (수신자 1명당 1회)
	void OnPacketSend() { ++_sendCount; }

	// 송신 큐가 가득 차서 버려진 패킷 수
	void OnPacketSendDrop() { ++_sendDropCount; }
	// ============================================================

	// ======================== 주기 처리 함수 ========================
	// 매 루프마다 호출 (EndFrame 뒤)
	void Update(size_t userCount)
	{
		_lastUserCount = userCount;

		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);

		double sinceCheckSec = Elapsed(_lastCheckTime, now);
		if (sinceCheckSec >= 1.0)
		{
			SampleSecond(sinceCheckSec, userCount);
			_lastCheckTime = now;
		}

		if (Elapsed(_lastWindowTime, now) >= _windowSec)
		{
			CloseWindow(now);
			_lastWindowTime = now;
		}

		if (Elapsed(_lastSaveTime, now) >= _saveSec)
		{
			SaveToFile();
			_lastSaveTime = now;
		}
	}

	void Flush()
	{
		if (!_initialized)
		{
			return;
		}

		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);

		// 1초가 안 찬 나머지 구간도 샘플 1개로 반영 (안 하면 마지막 줄 TPS/패킷이 0으로 찍힘)
		double sinceCheckSec = Elapsed(_lastCheckTime, now);
		if (sinceCheckSec >= 0.1 && _tickCount > 0)
		{
			SampleSecond(sinceCheckSec, _lastUserCount);
			_lastCheckTime = now;
		}

		// 1초 샘플이 하나도 없는 구간(0.1초 미만 자투리)은 버림 → TPS 0짜리 가짜 줄 방지
		if (_tpsWindow.Count() > 0)
		{
			CloseWindow(now);
			_lastWindowTime = now;
		}
		SaveToFile();
	}
	// ============================================================
private:
	// CSV 한 줄 = 1 구간(기본 1분)
	struct Row
	{
		time_t _wallTime;		// 구간이 끝난 시각
		double _elapsedSec;		// 서버 시작 후 경과 시간
		MinAvgMax _users;		// 접속자 수 (1초 샘플)
		MinAvgMax _frameMs;		// 프레임 처리 시간 (프레임 단위, ms)
		MinAvgMax _tps;			// 초당 프레임 수
		MinAvgMax _recvPerSec;	// 초당 recv 패킷
		MinAvgMax _sendPerSec;	// 초당 send 패킷
		MinAvgMax _dropPerSec;	// 초당 send drop
		ll _acceptTry;			// 구간 내 accept 시도 총합
		ll _acceptOk;			// 구간 내 accept 성공 총합
	};

	// 시간 구하기
	double Elapsed(LARGE_INTEGER from, LARGE_INTEGER to) const
	{
		return (double)(to.QuadPart - from.QuadPart) / _freq.QuadPart;
	}

	// 1초마다: 초당 횟수를 샘플로 만들어 구간 누적기에 넣고 콘솔 출력
	void SampleSecond(double sec, size_t userCount)
	{
		double tps = _tickCount / sec;
		double recvPerSec = _recvCount / sec;
		double sendPerSec = _sendCount / sec;
		double dropPerSec = _sendDropCount / sec;

		_usersWindow.Add((double)userCount);
		_tpsWindow.Add(tps);
		_recvWindow.Add(recvPerSec);
		_sendWindow.Add(sendPerSec);
		_dropWindow.Add(dropPerSec);
		_acceptTryWindow += _acceptCount;
		_acceptOkWindow += _acceptSuccessCount;

		printf("[Users: %zu] [TPS: %.1f] [Frame ms min/avg/max: %.4f / %.4f / %.4f] "
			"[Accept: %lld / OK %lld] [Recv/s: %.1f] [Send/s: %.1f] [Drop/s: %.1f]\n",
			userCount, tps,
			_frameMsSecond.Min(), _frameMsSecond.Avg(), _frameMsSecond.Max(),
			_acceptCount, _acceptSuccessCount,
			recvPerSec, sendPerSec, dropPerSec);

		_frameMsSecond.Reset();
		_tickCount = 0;
		_acceptCount = 0;
		_acceptSuccessCount = 0;
		_recvCount = 0;
		_sendCount = 0;
		_sendDropCount = 0;
	}

	// 구간 종료: 누적기 스냅샷을 버퍼에 넣고 리셋
	void CloseWindow(LARGE_INTEGER now)
	{
		Row row;
		row._wallTime = time(nullptr);
		row._elapsedSec = Elapsed(_startTime, now);
		row._users = _usersWindow;
		row._frameMs = _frameMsWindow;
		row._tps = _tpsWindow;
		row._recvPerSec = _recvWindow;
		row._sendPerSec = _sendWindow;
		row._dropPerSec = _dropWindow;
		row._acceptTry = _acceptTryWindow;
		row._acceptOk = _acceptOkWindow;
		_buffer.push_back(row);

		_usersWindow.Reset();
		_frameMsWindow.Reset();
		_tpsWindow.Reset();
		_recvWindow.Reset();
		_sendWindow.Reset();
		_dropWindow.Reset();
		_acceptTryWindow = 0;
		_acceptOkWindow = 0;
	}

	static void WriteStat(FILE* fp, const MinAvgMax& s, int precision)
	{
		fprintf(fp, ",%.*f,%.*f,%.*f", precision, s.Min(), precision, s.Avg(), precision, s.Max());
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
			fprintf(fp,
				"Time,ElapsedSec,"
				"Users_Min,Users_Avg,Users_Max,"
				"FrameMs_Min,FrameMs_Avg,FrameMs_Max,"
				"TPS_Min,TPS_Avg,TPS_Max,"
				"RecvPerSec_Min,RecvPerSec_Avg,RecvPerSec_Max,"
				"SendPerSec_Min,SendPerSec_Avg,SendPerSec_Max,"
				"DropPerSec_Min,DropPerSec_Avg,DropPerSec_Max,"
				"AcceptTry,AcceptOK\n");
		}

		for (const Row& row : _buffer)
		{
			tm lt;
			localtime_s(&lt, &row._wallTime);

			fprintf(fp, "%02d:%02d:%02d,%.1f", lt.tm_hour, lt.tm_min, lt.tm_sec, row._elapsedSec);
			WriteStat(fp, row._users, 1);
			WriteStat(fp, row._frameMs, 4);
			WriteStat(fp, row._tps, 1);
			WriteStat(fp, row._recvPerSec, 1);
			WriteStat(fp, row._sendPerSec, 1);
			WriteStat(fp, row._dropPerSec, 1);
			fprintf(fp, ",%lld,%lld\n", row._acceptTry, row._acceptOk);
		}

		fclose(fp);
		_buffer.clear();
	}

	// 정밀한 시간 측정
	LARGE_INTEGER _freq {};
	LARGE_INTEGER _startTime {};
	LARGE_INTEGER _lastCheckTime {};
	LARGE_INTEGER _lastWindowTime {};
	LARGE_INTEGER _lastSaveTime {};

	// 프레임 측정
	LARGE_INTEGER _frameStart {};
	LARGE_INTEGER _waitStart {};
	ll _waitTicks = 0;
	bool _inFrame = false;

	// 1초 카운터
	ll _tickCount = 0;
	ll _acceptCount = 0;
	ll _acceptSuccessCount = 0;
	ll _recvCount = 0;
	ll _sendCount = 0;
	ll _sendDropCount = 0;
	MinAvgMax _frameMsSecond;

	// 구간(기본 1분) 누적기
	MinAvgMax _usersWindow;
	MinAvgMax _frameMsWindow;
	MinAvgMax _tpsWindow;
	MinAvgMax _recvWindow;
	MinAvgMax _sendWindow;
	MinAvgMax _dropWindow;
	ll _acceptTryWindow = 0;
	ll _acceptOkWindow = 0;

	double _windowSec;
	double _saveSec;
	size_t _lastUserCount = 0;
	bool _initialized = false;
	std::vector<Row> _buffer;
	char _fileName[64] = {};
};