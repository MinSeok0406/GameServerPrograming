#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define FD_SETSIZE 10000
#include <iostream>
#include <time.h>
#include <list>
#include <string>
#include <algorithm>
#include "Protocol.h"
#include "MetricLogger.h"
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

#define SERVERPORT      47000

// 틱 소요시간 로그를 남길 CSV 경로 - MetricLogger와는 별개로 관리
#define TICK_LOG_CSV_PATH "server_tick_log.csv"

// 검증 관련 지표
MetricLogger g_metricLogger(60.0 * 60.0 * 1);

// 프리리스트 추가
#define MAX_PENDING_PER_USER 20000
ObjectFreeList<SerializationBuffer>     g_bufferPool(1000, false);
ObjectFreeList<PendingPacket>           g_pendingPool(3000, true);
ObjectFreeList<SendItem>                g_sendItemPool(10000, true);

list<USER> g_userList;
SOCKET g_listensocket;
SOCKET g_clientsocket;
SOCKADDR_IN clientaddr;

bool g_shutdown = false;

static int s_id = 0;

// 틱 소요시간 집계용 (QueryPerformanceCounter 기반, 고해상도)
static LARGE_INTEGER s_qpcFreq;
static double s_tickSumMs = 0.0;
static double s_tickMaxMs = 0.0;
static uint64_t s_tickCount = 0;

bool networkLogic();
bool Update();

// 로그 함수
void LogStatus();

// CSV 저장용 헬퍼
static bool FileExists(const char* path);
static void AppendTickLogCsv(int hour, int min, int sec, size_t userCount,
    double avgMs, double maxMs, uint64_t tickCount);

// 네트워크 함수 -> accepct, send, recv...
bool netProc_Accept();
bool netProc_Send(USER* user);
bool netProc_Recv(USER* user);
bool packetProc(USER* user, unsigned char type, SerializationBuffer* packet);
bool sendPacket_Unicast(USER* user, SerializationBuffer* packet);
bool sendPacket_Broadcast(USER* user, SerializationBuffer* packet);

// 네트워크 패킷 함수
bool netPacketProc_MSG(USER* user, SerializationBuffer* packet);

// 네트워크 프로토콜 함수
bool npf_SC_CREATE_USER(SerializationBuffer* packet, unsigned int id, int nameSize, char name[20]);
bool npf_SC_OTHER_USER(SerializationBuffer* packet, unsigned int id, int nameSize, char name[20]);
bool npf_SC_MSG(SerializationBuffer* packet, unsigned int senderID, unsigned int seq, unsigned long long sendTick,
    unsigned short len, unsigned int namesize, char name[20], char* msg);

// 프리리스트 관련 함수
void pushSendItem(USER* user, PendingPacket* pp);
void cleanupUserSendQueue(USER* user);

int wmain()
{
    timeBeginPeriod(1);
    srand((unsigned int)(time(nullptr)));

    g_metricLogger.Init();
    QueryPerformanceFrequency(&s_qpcFreq);

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("%d\n", WSAGetLastError());
        return 0;
    }

    g_listensocket = socket(AF_INET, SOCK_STREAM, 0);
    if (g_listensocket == INVALID_SOCKET)
    {
        printf("%d\n", WSAGetLastError());
        return 0;
    }

    SOCKADDR_IN serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    int bindRet = bind(g_listensocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (bindRet == SOCKET_ERROR)
    {
        printf("%d\n", WSAGetLastError());
        return 0;
    }

    int listenRet = listen(g_listensocket, SOMAXCONN_HINT(10000));
    if (listenRet == SOCKET_ERROR)
    {
        printf("%d\n", WSAGetLastError());
        return 0;
    }

    u_long on = 1;
    int nonbloRet = ioctlsocket(g_listensocket, FIONBIO, &on);
    if (nonbloRet == SOCKET_ERROR)
    {
        printf("%d\n", WSAGetLastError());
        return 0;
    }

    while (!g_shutdown)
    {
        /*LARGE_INTEGER tickStart, tickEnd;
        QueryPerformanceCounter(&tickStart);*/

        networkLogic();
        Update();

        /*QueryPerformanceCounter(&tickEnd);
        double ms = (double)(tickEnd.QuadPart - tickStart.QuadPart) * 1000.0 / (double)s_qpcFreq.QuadPart;
        s_tickSumMs += ms;
        if (ms > s_tickMaxMs) s_tickMaxMs = ms;
        s_tickCount++;*/
    }
    
    WSACleanup();

    return 1;
}

bool networkLogic()
{
    fd_set rset;
    fd_set wset;

    FD_ZERO(&rset);
    FD_ZERO(&wset);

    FD_SET(g_listensocket, &rset);

    for (auto& user : g_userList)
    {
        FD_SET(user._sock, &rset);
        if (user._sendHead != nullptr)
        {
            FD_SET(user._sock, &wset);
        }
    }

    int selectRet = select(0, &rset, &wset, 0, NULL);
    if (selectRet == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAEWOULDBLOCK)
        {
            printf("%d\n", WSAGetLastError());
            g_shutdown = true;
        }
        
        return false;
    }

    if (selectRet > 0)
    {
        if (FD_ISSET(g_listensocket, &rset))
        {
            netProc_Accept();
        }

        for (auto it = g_userList.begin(); it != g_userList.end();)
        {
            USER& user = *it;

            if (FD_ISSET(user._sock, &rset))
            {
                selectRet--;
                netProc_Recv(&user);
            }

            if (FD_ISSET(user._sock, &wset))
            {
                selectRet--;
                netProc_Send(&user);
            }

            if (user._disconnected)
            {
                cleanupUserSendQueue(&user);
                closesocket(user._sock);
                it = g_userList.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    return true;
}

bool Update()
{
    // 채팅 로그
    // DB 저장
    // 등등
    /*g_metricLogger.OnTick();
    g_metricLogger.Update();

    LogStatus();*/

    return true;
}

// CSV 파일이 이미 있는지 확인 (있으면 헤더를 다시 안 씀)
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

// 틱 소요시간 로그를 CSV에 한 줄 추가
static void AppendTickLogCsv(int hour, int min, int sec, size_t userCount,
    double avgMs, double maxMs, uint64_t tickCount)
{
    bool needHeader = !FileExists(TICK_LOG_CSV_PATH);
    FILE* fp;
    fopen_s(&fp, TICK_LOG_CSV_PATH, "a");
    if (fp == nullptr)
    {
        printf("CSV 저장 실패: %s 파일을 열 수 없습니다\n", TICK_LOG_CSV_PATH);
        return;
    }

    if (needHeader)
    {
        fprintf(fp, "time,user_count,tick_avg_ms,tick_max_ms,tick_count\n");
    }

    fprintf(fp, "%02d:%02d:%02d,%zu,%.3f,%.3f,%llu\n",
        hour, min, sec, userCount, avgMs, maxMs, (unsigned long long)tickCount);

    fclose(fp);
}

void LogStatus()
{
    static uint64_t s_lastLogTick = 0;
    uint64_t now = GetTickCount64();

    // 60000ms = 1분 안 지났으면 그냥 리턴
    if (now - s_lastLogTick < 10000)
    {
        return;
    }
    s_lastLogTick = now;

    time_t nowTime = time(nullptr);
    tm localTm;
    localtime_s(&localTm, &nowTime);

    double avgMs = (s_tickCount > 0) ? (s_tickSumMs / (double)s_tickCount) : 0.0;

    printf("[%02d:%02d:%02d] userList size = %zu | tick avg=%.3fms max=%.3fms (n=%llu)\n",
        localTm.tm_hour, localTm.tm_min, localTm.tm_sec, g_userList.size(),
        avgMs, s_tickMaxMs, (unsigned long long)s_tickCount);

    // 같은 값을 CSV에도 한 줄 추가 (10초마다 쌓여서 실행 시간 동안의 추이를 그래프로 볼 수 있음)
    AppendTickLogCsv(localTm.tm_hour, localTm.tm_min, localTm.tm_sec,
        g_userList.size(), avgMs, s_tickMaxMs, s_tickCount);

    // 다음 10초 구간을 위해 리셋
    s_tickSumMs = 0.0;
    s_tickMaxMs = 0.0;
    s_tickCount = 0;
}

bool netProc_Accept()
{
    //g_metricLogger.OnTickAccept();
    int addrlen = sizeof(clientaddr);
    g_clientsocket = accept(g_listensocket, (SOCKADDR*)&clientaddr, &addrlen);
    if (g_clientsocket == INVALID_SOCKET)
    {
        if (WSAGetLastError() != WSAEWOULDBLOCK)
        {
            printf("accept error : %d\n", WSAGetLastError());
        }
        
        return false;
    }

    g_userList.emplace_back();
    USER& createuser = g_userList.back();
    createuser._id = s_id;
    createuser._sock = g_clientsocket;
    string name = "User" + to_string(s_id++);
    createuser._namesize = (unsigned int)name.size();
    memcpy(createuser._name, name.c_str(), createuser._namesize);
    InetNtop(AF_INET, &clientaddr.sin_addr, createuser._ip, sizeof(createuser._ip));
    createuser._port = ntohs(clientaddr.sin_port);
    createuser._disconnected = false;
    
    // 신규 유저 정보 전송
    SerializationBuffer packet;
    npf_SC_CREATE_USER(&packet, createuser._id, createuser._namesize, createuser._name);
    sendPacket_Unicast(&createuser, &packet);
    packet.clear();

    // 기존 유저들에게 신규 유저 정보 전송
    npf_SC_OTHER_USER(&packet, createuser._id, createuser._namesize, createuser._name);
    sendPacket_Broadcast(&createuser, &packet);
    packet.clear();

    // 신규 유저에게 기존 유저들 정보 전송
    for (auto& users : g_userList)
    {
        if (users._id != createuser._id)
        {
            npf_SC_OTHER_USER(&packet, users._id, users._namesize, users._name);
            sendPacket_Unicast(&createuser, &packet);
            packet.clear();
        }
    }

    //g_metricLogger.OnTickSuccess();
    return true;
}

bool netProc_Send(USER* user)
{
    while (user->_sendHead != nullptr)
    {
        SendItem* item = user->_sendHead;
        PendingPacket* pp = item->packet;

        int total = pp->buf->getDataSize();
        int remain = total - item->sentOffset;

        int sendRet = send(user->_sock, pp->buf->getBufferPtr() + item->sentOffset, remain, 0);
        if (sendRet == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSAEWOULDBLOCK)
            {
                printf("send error : %d\n", WSAGetLastError());
                user->_disconnected = true;
            }
            break;
        }

        item->sentOffset += sendRet;

        // 부분 전송, 다음 틱에 이어서
        if (item->sentOffset < total)
        {
            break;
        }

        // 이 유저 몫은 끝 -> 큐에서 빼고 참조 해제
        user->_sendHead = item->next;
        if (!user->_sendHead)
        {
            user->_sendTail = nullptr;
        }
        user->_sendQueueCount--;

        if (--(pp->refCount) == 0)
        {
            g_bufferPool.Free(pp->buf);
            g_pendingPool.Free(pp);
        }
        g_sendItemPool.Free(item);
    }

    return true;
}

bool netProc_Recv(USER* user)
{
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
            printf("recv error : %d\n", WSAGetLastError());
            user->_disconnected = true;
        }

        return true;
    }
    else if (recvRet == 0)
    {
        user->_disconnected = true;
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
        int peekRet = user->_recvQ.Peek(buf, sizeof(HEADER));
        if (peekRet != sizeof(HEADER))
        {
            __debugbreak();
        }

        HEADER* header = (HEADER*)buf;
        if (user->_recvQ.GetUseSize() < sizeof(HEADER) + header->_packetsize)
        {
            return false;
        }

        user->_recvQ.MoveFront(peekRet);

        SerializationBuffer packet;
        peekRet = user->_recvQ.Peek(packet.getBufferPtr(), header->_packetsize);
        if (peekRet != header->_packetsize)
        {
            __debugbreak();
        }
        user->_recvQ.MoveFront(peekRet);
        packet.moveReadPos(peekRet);

        packetProc(user, header->_type, &packet);
    }

    return true;
}

bool packetProc(USER* user, unsigned char type, SerializationBuffer* packet)
{
    //g_metricLogger.OnPacketRecv();

    switch (type)
    {
    case PACKET_CS_MSG:
        netPacketProc_MSG(user, packet);
        break;
    }

    return true;
}

bool sendPacket_Unicast(USER* user, SerializationBuffer* packet)
{
    if (user->_sendQueueCount >= MAX_PENDING_PER_USER)
    {
        //g_metricLogger.OnPacketSendDrop();
        return false;
    }

    //g_metricLogger.OnPacketSend();
    SerializationBuffer* pooled = g_bufferPool.Alloc();
    pooled->clear();
    pooled->putData(packet->getBufferPtr(), packet->getDataSize());

    PendingPacket* pp = g_pendingPool.Alloc();
    pp->buf = pooled;
    pp->refCount = 1;

    pushSendItem(user, pp);

    return true;
}

bool sendPacket_Broadcast(USER* user, SerializationBuffer* packet)
{
    static vector<USER*> recipients;
    recipients.clear();

    for (auto& u : g_userList)
    {
        if (u._id == user->_id)
        {
            continue;
        }

        // 수정 필요
        if (u._sendQueueCount >= MAX_PENDING_PER_USER)
        {
            //g_metricLogger.OnPacketSendDrop();
            continue;
        }
        recipients.push_back(&u);
    }
    
    if (recipients.empty())
    {
        return true;
    }

    SerializationBuffer* pooled = g_bufferPool.Alloc();
    pooled->clear();
    pooled->putData(packet->getBufferPtr(), packet->getDataSize());

    PendingPacket* pp = g_pendingPool.Alloc();
    pp->buf = pooled;
    pp->refCount = (uint32_t)recipients.size();

    for (USER* u : recipients)
    {
        pushSendItem(u, pp);
        //g_metricLogger.OnPacketSend();
    }

    return true;
}

bool netPacketProc_MSG(USER* user, SerializationBuffer* packet)
{
    // 지표 관련 패킷 메시지(프로토콜와는 관련 없음)
    unsigned int seq = 0;
    unsigned long long sendTick = 0;

    unsigned short len;
    unsigned int namesize;
    char name[20];
    char msg[500];

    //*packet >> seq;
    //*packet >> sendTick;
    *packet >> len;
    *packet >> namesize;
    packet->getData(name, namesize);
    name[namesize] = '\0';

    packet->getData(msg, len);
    msg[len] = '\0';

    // 추가할 예정있다면 작성
    printf("%s : %s\n", name, msg);
    
    SerializationBuffer sendPacket;
    npf_SC_MSG(&sendPacket, user->_id, seq, sendTick, len, namesize, name, msg);
    sendPacket_Broadcast(user, &sendPacket);

    return true;
}

bool npf_SC_CREATE_USER(SerializationBuffer* packet, unsigned int id, int nameSize, char name[20])
{
    HEADER header;
    header._packetsize = (unsigned short)(sizeof(id) + sizeof(nameSize) + nameSize);
    header._type = PACKET_SC_CREATE_USER;

    packet->putData((char*)&header, sizeof(header));

    *packet << id;
    *packet << nameSize;
    packet->putData(name, nameSize);

    return true;
}

bool npf_SC_OTHER_USER(SerializationBuffer* packet, unsigned int id, int nameSize, char name[20])
{
    HEADER header;
    header._packetsize = (unsigned short)(sizeof(id) + sizeof(nameSize) + nameSize);
    header._type = PACKET_SC_OTHER_USER;

    packet->putData((char*)&header, sizeof(header));

    *packet << id;
    *packet << nameSize;
    packet->putData(name, nameSize);

    return true;
}

bool npf_SC_MSG(SerializationBuffer* packet, unsigned int senderID, unsigned int seq, unsigned long long sendTick,
    unsigned short len, unsigned int namesize, char name[20], char* msg)
{
    HEADER header;
    /*header._packetsize = (unsigned short)(sizeof(senderID) + sizeof(seq) + sizeof(sendTick) +
        sizeof(len) + sizeof(namesize) + namesize + len);*/
    header._packetsize = (unsigned short)(sizeof(len) + sizeof(namesize) + namesize + len);
    header._type = PACKET_SC_MSG;

    packet->putData((char*)&header, sizeof(header));

    //*packet << senderID;
    //*packet << seq;
    //*packet << sendTick;
    *packet << len;
    *packet << namesize;
    packet->putData(name, namesize);
    packet->putData(msg, len);

    return true;
}

void pushSendItem(USER* user, PendingPacket* pp)
{
    SendItem* item = g_sendItemPool.Alloc();
    item->packet = pp;
    item->sentOffset = 0;
    item->next = nullptr;

    if (user->_sendTail)
    {
        user->_sendTail->next = item;
    }
    else
    {
        user->_sendHead = item;
    }

    user->_sendTail = item;
    user->_sendQueueCount++;
}

void cleanupUserSendQueue(USER * user)
{
    SendItem* item = user->_sendHead;
    while (item != nullptr)
    {
        SendItem* next = item->next;
        PendingPacket* pp = item->packet;
        if (--(pp->refCount) == 0)
        {
            g_bufferPool.Free(pp->buf);
            g_pendingPool.Free(pp);
        }
        g_sendItemPool.Free(item);
        item = next;
    }
    user->_sendHead = nullptr;
    user->_sendTail = nullptr;
    user->_sendQueueCount = 0;
}