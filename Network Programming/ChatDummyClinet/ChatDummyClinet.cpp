#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <time.h>
#include <list>
#include <string>
#include <algorithm>
#include <unordered_map>
#include "Protocol.h"
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

/*const wchar_t* SERVERIP = L"59.14.69.45";
#define SERVERPORT 51000*/
const wchar_t* SERVERIP = L"127.0.0.1";
#define SERVERPORT 47000

#define CLIENT_COUNT		3000
#define MIN_INTERVAL_MS		200					// 클라이언트 1개당 최소 발송 주기(ms)
#define MAX_INTERVAL_MS		200					// 클라이언트 1개당 최대 발송 주기(ms)
#define TEST_DURATION_SEC	300					// 이 시간(초)이 지나면 전체 클라이언트를 정상 종료시키고 요약 출력
#define RESULT_CSV_PATH		"stress_test_results.csv"

static atomic<bool> g_testShutdown { false };
static atomic<int> g_readyCount { 0 };
static atomic<bool> g_allConnected { false };

// 패킷 유실 번호 확인용 구조체
struct SeqTrackInfo
{
	uint32_t lastSeq = 0;
	bool bseenFirst = false;
	uint64_t lostCount = 0;
	uint64_t recvCount = 0;
};

struct ClientContext
{
	USER user {};
	unsigned int sendSeq = 0;
	unordered_map<unsigned int, SeqTrackInfo> lastSeqBySender;
};

// 전체 집계용 통계 (모든 스레드가 공유, atomic으로 보호)
struct GlobalStats
{
	atomic<uint64_t> totalReceived { 0 };
	atomic<uint64_t> totalLostEstimate { 0 };     // seq 간격으로 추정한 유실 메시지 수
	atomic<uint64_t> totalReorderDetected { 0 };  // 순서가 뒤바뀐 것으로 보이는 횟수
	atomic<uint64_t> latencySumMs { 0 };
	atomic<uint64_t> latencyMaxMs { 0 };
	atomic<uint64_t> latencyCount { 0 };
	atomic<uint64_t> bucket_0_10 { 0 };
	atomic<uint64_t> bucket_10_50 { 0 };
	atomic<uint64_t> bucket_50_200 { 0 };
	atomic<uint64_t> bucket_200_1000 { 0 };
	atomic<uint64_t> bucket_1000_plus { 0 };
};

static GlobalStats g_stats;
static atomic<uint64_t> g_totalSent { 0 };
static atomic<uint64_t> g_connectFailCount { 0 };

bool netProc_Recv(ClientContext* ctx);
bool netProc_Send(USER* user);
bool sendPacket_Unicast(SerializationBuffer* packet, USER* user);
bool packetProc(unsigned char type, SerializationBuffer* packet, ClientContext* ctx);

bool networkLogic(ClientContext* ctx);
bool Update(ClientContext* ctx);

bool netPacketProc_CreateUser(SerializationBuffer* packet, ClientContext* ctx);
bool netPacketProc_OtherUser(SerializationBuffer* packet);
bool netPacketProc_MSG(SerializationBuffer* packet, ClientContext* ctx);

bool npfMSG(SerializationBuffer* packet, unsigned int seq, unsigned long long sendTick,
	unsigned short len, unsigned int namesize, char name[20], char* msg);

void OnMessageRecv(ClientContext* ctx, unsigned int sender, uint32_t incomingSeq);
void PrintFinalStats(int clientCount, uint64_t totalSent, uint64_t connectFailCount);

unsigned int WINAPI threadProc(PVOID arg)
{
	ClientContext ctx;

	ctx.user._sock = socket(AF_INET, SOCK_STREAM, 0);
	if (ctx.user._sock == INVALID_SOCKET)
	{
		printf("%d\n", WSAGetLastError());
		return 1;
	}

	SOCKADDR_IN serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	InetPton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);

	int connectRet = connect(ctx.user._sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (connectRet == SOCKET_ERROR)
	{
		printf("%d\n", WSAGetLastError());
		g_connectFailCount.fetch_add(1, memory_order_relaxed); // [추가] 연결 실패 집계
		closesocket(ctx.user._sock);
		return 1;
	}

	u_long on = 1;
	int nonblkRet = ioctlsocket(ctx.user._sock, FIONBIO, &on);
	if (nonblkRet == SOCKET_ERROR)
	{
		printf("%d\n", WSAGetLastError());
		closesocket(ctx.user._sock);
		return 1;
	}

	while (!ctx.user._disconnect && !g_testShutdown && !g_allConnected)
	{
		networkLogic(&ctx);
	}

	int range = MAX_INTERVAL_MS - MIN_INTERVAL_MS + 1;
	while (!ctx.user._disconnect && !g_testShutdown)
	{
		int randNum = MIN_INTERVAL_MS + (range > 0 ? (rand() % range) : 0);
		networkLogic(&ctx);
		Update(&ctx);
		Sleep(randNum);
	}

	for (auto& [sender, info] : ctx.lastSeqBySender)
	{
		g_stats.totalLostEstimate.fetch_add(info.lostCount, memory_order_relaxed);
	}

	closesocket(ctx.user._sock);

	return 1;
}

int wmain()
{
	srand((unsigned int)time(nullptr));
	timeBeginPeriod(1);

	// 이번 설정(#define)으로 서버가 받게 될 이론적 부하를 미리 계산해서 보여줌
	// (클라이언트 수를 늘리면 브로드캐스트 팬아웃 때문에 총 송신량은 N^2로 늘어남)
	double avgIntervalSec = ((MIN_INTERVAL_MS + MAX_INTERVAL_MS) / 2.0) / 1000.0;
	double perClientRate = 1.0 / avgIntervalSec;
	double totalSendRate = perClientRate * CLIENT_COUNT;
	double totalBroadcastRate = totalSendRate * (double)(CLIENT_COUNT > 1 ? CLIENT_COUNT - 1 : 0);

	printf("=== 이번 실행 설정 ===\n");
	printf("클라이언트 수: %d\n", CLIENT_COUNT);
	printf("발송 주기: %d~%dms (평균 %.0fms)\n", MIN_INTERVAL_MS, MAX_INTERVAL_MS, avgIntervalSec * 1000);
	printf("클라이언트 1개당 초당 발송: 약 %.2f건\n", perClientRate);
	printf("서버가 받는 초당 총 발송(CS_MSG): 약 %.1f건\n", totalSendRate);
	printf("브로드캐스트로 인해 서버가 내보내야 하는 초당 총량(SC_MSG, N-1 팬아웃): 약 %.1f건\n", totalBroadcastRate);
	printf("======================\n\n");

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}

	HANDLE hThread[CLIENT_COUNT];
	for (auto i = 0; i < CLIENT_COUNT; ++i)
	{
		hThread[i] = (HANDLE)_beginthreadex(NULL, 0, threadProc, NULL, NULL, NULL);
		if (hThread[i] == 0)
		{
			return 1;
		}
	}

	printf("모든 클라이언트 접속 대기 중...\n");
	while (g_readyCount.load() < CLIENT_COUNT)
	{
		Sleep(50);
	}
	g_allConnected = true;

	// 정해진 시간만큼 테스트를 진행한 뒤 전체 클라이언트에게 종료 신호를 보냄.
	// 이렇게 해야 단계별 테스트를 매번 같은 시간만큼 돌려서 공정하게 비교할 수 있음
	printf("전원 접속 완료 - %d초 동안 부하 테스트를 진행합니다...\n", TEST_DURATION_SEC);
	Sleep(TEST_DURATION_SEC * 1000);
	g_testShutdown = true;
	printf("테스트 종료 신호 전송 - 클라이언트들이 정리되는 중...\n");

	for (auto i = 0; i < CLIENT_COUNT; ++i)
	{
		auto waitForRet = WaitForSingleObject(hThread[i], INFINITE);
		if (waitForRet == WAIT_FAILED)
		{
			printf("WaitForSingleObject failed : %lu\n", GetLastError());
		}
	}

	for (auto i = 0; i < CLIENT_COUNT; ++i)
	{
		CloseHandle(hThread[i]);
	}

	WSACleanup();

	PrintFinalStats(CLIENT_COUNT, g_totalSent.load(), g_connectFailCount.load());

	return 0;
}

// 이미 파일이 있는지 확인 (있으면 헤더를 다시 안 씀)
static bool FileExists(const char* path)
{
	FILE* fp;
	fopen_s(&fp, path, "r");
	if (fp)
	{
		fclose(fp);
		return true;
	}
	return false;
}

void OnMessageRecv(ClientContext* ctx, unsigned int sender, uint32_t incomingSeq)
{
	auto& info = ctx->lastSeqBySender[sender];

	if (!info.bseenFirst)
	{
		info.lastSeq = incomingSeq;
		info.bseenFirst = true;
		info.recvCount = 1;
		return;
	}

	if (incomingSeq == info.lastSeq + 1)
	{
		info.lastSeq = incomingSeq;
	}
	else if (incomingSeq > info.lastSeq + 1)
	{
		info.lostCount += incomingSeq - info.lastSeq - 1;
		info.lastSeq = incomingSeq;
	}
	else
	{
		g_stats.totalReorderDetected.fetch_add(1, memory_order_relaxed);
	}
	info.recvCount++;
}

// 전체 실행이 끝난 뒤 집계된 검증/성능 지표 출력
void PrintFinalStats(int clientCount, uint64_t totalSent, uint64_t connectFailCount)
{
	uint64_t totalReceived = g_stats.totalReceived.load();
	uint64_t lostEstimate = g_stats.totalLostEstimate.load();
	uint64_t reorder = g_stats.totalReorderDetected.load();
	uint64_t latencyCount = g_stats.latencyCount.load();
	uint64_t latencySum = g_stats.latencySumMs.load();
	uint64_t latencyMax = g_stats.latencyMaxMs.load();
	double latencyAvg = (latencyCount > 0) ? (double)latencySum / (double)latencyCount : 0.0;
	double lossRatePercent = (totalReceived + lostEstimate > 0) ? (100.0 * (double)lostEstimate / (double)(totalReceived + lostEstimate)) : 0.0;

	printf("\n=== 테스트 결과 요약 ===\n");
	printf("설정 클라이언트 수: %d, 연결 실패: %llu\n", clientCount, connectFailCount);
	printf("총 발송 시도: %llu\n", totalSent);
	printf("총 수신: %llu\n", totalReceived);
	printf("유실 추정: %llu건", lostEstimate);
	if (totalSent > 0)
	{
		printf(" (발송 대비 %.4f%%)", lossRatePercent);
	}
	printf("\n");
	printf("순서꼬임 감지: %llu건\n", reorder);
	printf("지연시간 - 평균: %.2fms, 최대: %llums  (※ 클라이언트-서버 동일 PC 실행 기준일 때만 유효)\n",
		latencyAvg, latencyMax);
	printf("지연시간 분포: 0-10ms=%llu  10-50ms=%llu  50-200ms=%llu  200-1000ms=%llu  1000ms+=%llu\n",
		g_stats.bucket_0_10.load(),
		g_stats.bucket_10_50.load(),
		g_stats.bucket_50_200.load(),
		g_stats.bucket_200_1000.load(),
		g_stats.bucket_1000_plus.load());
	printf("========================\n");

	// ---- CSV로 누적 저장 (실행할 때마다 한 줄씩 추가) ----
	// exe가 실행되는 위치(작업 디렉토리) 기준 상대 경로에 생성됨
	bool needHeader = !FileExists(RESULT_CSV_PATH);
	FILE* fp;
	fopen_s(&fp, RESULT_CSV_PATH, "a");
	if (fp == nullptr)
	{
		printf("CSV 저장 실패: %s 파일을 열 수 없습니다\n", RESULT_CSV_PATH);
		return;
	}

	if (needHeader)
	{
		fprintf(fp,
			"timestamp,client_count,min_interval_ms,max_interval_ms,test_duration_sec,"
			"connect_fail,total_sent,total_received,lost_estimate,loss_rate_percent,reorder_detected,"
			"latency_avg_ms,latency_max_ms,bucket_0_10,bucket_10_50,bucket_50_200,bucket_200_1000,bucket_1000_plus\n");
	}

	time_t nowTime = time(nullptr);
	tm localTm;
	localtime_s(&localTm, &nowTime);
	char timestamp[32];
	snprintf(timestamp, sizeof(timestamp), "%04d-%02d-%02d %02d:%02d:%02d",
		localTm.tm_year + 1900, localTm.tm_mon + 1, localTm.tm_mday,
		localTm.tm_hour, localTm.tm_min, localTm.tm_sec);

	fprintf(fp, "%s,%d,%d,%d,%d,%llu,%llu,%llu,%llu,%.4f,%llu,%.2f,%llu,%llu,%llu,%llu,%llu,%llu\n",
		timestamp,
		clientCount, MIN_INTERVAL_MS, MAX_INTERVAL_MS, TEST_DURATION_SEC,
		connectFailCount,
		totalSent,
		totalReceived,
		lostEstimate,
		lossRatePercent,
		reorder,
		latencyAvg,
		latencyMax,
		g_stats.bucket_0_10.load(),
		g_stats.bucket_10_50.load(),
		g_stats.bucket_50_200.load(),
		g_stats.bucket_200_1000.load(),
		g_stats.bucket_1000_plus.load());

	fclose(fp);

	printf("결과가 %s 파일에 누적 저장되었습니다.\n", RESULT_CSV_PATH);
	getchar();
}

bool netProc_Recv(ClientContext* ctx)
{
	USER* user = &ctx->user;

	if (user->_recvQ.GetFreeSize() < sizeof(HEADER))
	{
		return false;
	}

	int recvRet = recv(user->_sock, user->_recvQ.GetRearBufferPtr(),
		user->_recvQ.DirectEnqueueSize(), 0);
	if (recvRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			printf("recv fail: %d\n", WSAGetLastError());
		}

		return false;
	}
	else if (recvRet == 0)
	{
		user->_disconnect = true;
		return true;
	}

	user->_recvQ.MoveRear(recvRet);

	while (true)
	{
		if (user->_recvQ.GetUseSize() <= sizeof(HEADER))
		{
			return false;
		}

		char buf[3];
		uint32_t peekRet = user->_recvQ.Peek(buf, sizeof(HEADER));
		if (peekRet != sizeof(HEADER))
		{
			__debugbreak();
		}

		HEADER* header = (HEADER*)buf;
		if (user->_recvQ.GetUseSize() < sizeof(HEADER) + header->_packetsize)
		{
			return false;   // 헤더는 아직 큐에서 빼지 않은 상태로 리턴 -> 다음 recv에서 이어서 재확인
		}

		user->_recvQ.MoveFront(sizeof(HEADER));

		SerializationBuffer packet;
		peekRet = user->_recvQ.Peek(packet.getBufferPtr(), header->_packetsize);
		if (peekRet != header->_packetsize)
		{
			__debugbreak();
		}
		user->_recvQ.MoveFront(peekRet);
		packet.moveReadPos(peekRet);

		packetProc(header->_type, &packet, ctx);
	}

	return true;
}

bool netProc_Send(USER* user)
{
	while (true)
	{
		if (user->_sendQ.GetUseSize() < sizeof(HEADER))
		{
			break;
		}

		int sendRet = send(user->_sock, user->_sendQ.GetFrontBufferPtr(),
			user->_sendQ.DirectDequeueSize(), 0);
		if (sendRet == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				printf("%d\n", WSAGetLastError());
				printf("send fail\n");
			}
			break;
		}

		user->_sendQ.MoveFront(sendRet);
	}

	return true;
}

bool sendPacket_Unicast(SerializationBuffer* packet, USER* user)
{
	uint32_t size = packet->getDataSize();
	if (user->_sendQ.GetFreeSize() < size)
	{
		printf("send fail\n");
		return false;
	}

	uint32_t enqueueRet = user->_sendQ.Enqueue(packet->getBufferPtr(), size);
	if (enqueueRet != packet->getDataSize())
	{
		__debugbreak();
	}

	packet->moveWritePos(size);

	return true;
}

bool packetProc(unsigned char type, SerializationBuffer* packet, ClientContext* ctx)
{
	switch (type)
	{
	case PACKET_SC_CREATE_USER:
		netPacketProc_CreateUser(packet, ctx);
		break;
	case PACKET_SC_OTHER_USER:
		netPacketProc_OtherUser(packet);
		break;
	case PACKET_SC_MSG:
		netPacketProc_MSG(packet, ctx);
		break;
	}

	return true;
}

bool networkLogic(ClientContext* ctx)
{
	fd_set rset;
	fd_set wset;
	FD_ZERO(&rset);
	FD_ZERO(&wset);

	FD_SET(ctx->user._sock, &rset);
	if (ctx->user._sendQ.GetUseSize() > 0)
	{
		FD_SET(ctx->user._sock, &wset);
	}

	timeval t;
	t.tv_sec = 0;
	t.tv_usec = 100 * 1000;

	int selectRet = select(0, &rset, &wset, nullptr, &t);
	if (selectRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			printf("%d\n", WSAGetLastError());
		}

		return false;
	}

	if (selectRet > 0)
	{
		if (FD_ISSET(ctx->user._sock, &rset))
		{
			selectRet--;
			netProc_Recv(ctx);
		}

		if (FD_ISSET(ctx->user._sock, &wset))
		{
			selectRet--;
			netProc_Send(&ctx->user);
		}
	}

	return true;
}

bool Update(ClientContext* ctx)
{
	if (!ctx->user._ready)
	{
		return false;
	}

	static const char* messages[] = {
		"안녕하세요!!",
		"오늘 날씨 좋네요~",
		"스트레스 테스트 중입니다",
		"ㅋㅋㅋㅋㅋㅋ",
		"패킷 잘 도착하나요?"
	};
	int messageCount = sizeof(messages) / sizeof(messages[0]);

	char msg[500];
	strcpy(msg, messages[rand() % messageCount]);
	unsigned char len = (unsigned char)strlen(msg);

	// 검증용 일련번호 + 발송 시각
	unsigned int seq = ctx->sendSeq;
	unsigned long long sendTick = GetTickCount64();

	SerializationBuffer packet;
	npfMSG(&packet, seq, sendTick, len, ctx->user._namesize, ctx->user._name, msg);
	if (sendPacket_Unicast(&packet, &ctx->user))
	{
		ctx->sendSeq++;
		g_totalSent.fetch_add(1, memory_order_relaxed);
	}
	return true;
}

bool netPacketProc_CreateUser(SerializationBuffer* packet, ClientContext* ctx)
{
	SC_CREATE_USER createUser;
	*packet >> createUser._id;
	*packet >> createUser._namesize;
	packet->getData(createUser._name, createUser._namesize);

	ctx->user._id = createUser._id;
	ctx->user._namesize = createUser._namesize;
	memcpy(ctx->user._name, createUser._name, createUser._namesize);
	ctx->user._ready = true;
	ctx->user._disconnect = false;

	g_readyCount.fetch_add(1, memory_order_relaxed);

	return true;
}

bool netPacketProc_OtherUser(SerializationBuffer* packet)
{
	SC_OTHER_USER otherUser;
	*packet >> otherUser._id;
	*packet >> otherUser._namesize;
	packet->getData(otherUser._name, otherUser._namesize);

	return true;
}

// seq/sendTick/senderId를 파싱해서 지연시간, 유실, 순서꼬임을 검증함.
bool netPacketProc_MSG(SerializationBuffer* packet, ClientContext* ctx)
{
	unsigned int senderId;
	unsigned int seq;
	unsigned long long sendTick;
	unsigned short len;
	unsigned int namesize;
	char name[20];
	char msg[500];

	*packet >> senderId;
	*packet >> seq;
	*packet >> sendTick;
	*packet >> len;
	*packet >> namesize;
	packet->getData(name, namesize);
	name[namesize] = '\0';

	packet->getData(msg, len);
	msg[len] = '\0';

	// --- 지연시간 측정 (동일 PC 실행 기준) ---
	unsigned long long now = GetTickCount64();
	unsigned long long latencyMs = now - sendTick;
	if (latencyMs < 0) latencyMs = 0; // 클럭 오차 방어

	g_stats.totalReceived.fetch_add(1, memory_order_relaxed);
	g_stats.latencySumMs.fetch_add(latencyMs, memory_order_relaxed);
	g_stats.latencyCount.fetch_add(1, memory_order_relaxed);

	unsigned long long prevMax = g_stats.latencyMaxMs.load(memory_order_relaxed);
	while (latencyMs > prevMax && !g_stats.latencyMaxMs.compare_exchange_weak(prevMax, latencyMs))
	{}

	if (latencyMs < 10)
	{
		g_stats.bucket_0_10.fetch_add(1, memory_order_relaxed);
	}
	else if (latencyMs < 50)
	{
		g_stats.bucket_10_50.fetch_add(1, memory_order_relaxed);
	}
	else if (latencyMs < 200)
	{
		g_stats.bucket_50_200.fetch_add(1, memory_order_relaxed);
	}
	else if (latencyMs < 1000)
	{
		g_stats.bucket_200_1000.fetch_add(1, memory_order_relaxed);
	}
	else
	{
		g_stats.bucket_1000_plus.fetch_add(1, memory_order_relaxed);
	}

	// --- 발신자(senderId)별 seq 연속성 검사: 유실/순서꼬임 추정 ---
	OnMessageRecv(ctx, senderId, seq);

	return true;
}

bool npfMSG(SerializationBuffer* packet, unsigned int seq, unsigned long long sendTick,
	unsigned short len, unsigned int namesize, char name[20], char* msg)
{
	HEADER header;
	header._packetsize = (unsigned short)(sizeof(seq) + sizeof(sendTick) + sizeof(len) + sizeof(namesize) + namesize + len);
	header._type = PACKET_CS_MSG;

	packet->putData((char*)&header, sizeof(HEADER));

	*packet << seq;
	*packet << sendTick;
	*packet << len;
	*packet << namesize;
	packet->putData(name, namesize);
	packet->putData(msg, len);

	return true;
}
