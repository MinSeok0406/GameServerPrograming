#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <tchar.h>
#include <time.h>
#include <process.h>
#include <Windows.h>
#include <ws2tcpip.h>
using namespace std;
using ll = long long;

#pragma comment(lib, "Winmm.lib")

// InetPton -> IP 주소 문자열을 IN_ADDR 형태로 저장
// InetNtop -> IN_ADDR 형태의 정보를 IP 문자열로 변환

BOOL DomainToIP(WCHAR* szDomain, IN_ADDR* pAddr)
{
    ADDRINFOW* pAddrInfo;
    SOCKADDR_IN* pSockAddr[1024];

    if (GetAddrInfo(szDomain, L"0", NULL, &pAddrInfo) != 0)
    {
        return FALSE;
    }

    int cnt = 0;
    ADDRINFOW* temp = pAddrInfo;
    while (temp != nullptr)
    {
        pSockAddr[cnt++] = (SOCKADDR_IN*)temp->ai_addr;
        temp = temp->ai_next;
    }

    if (cnt == 0)
    {
        FreeAddrInfoW(pAddrInfo);
        return FALSE;
    }

    int randNum = rand() % cnt;
    *pAddr = pSockAddr[randNum]->sin_addr;
    FreeAddrInfoW(pAddrInfo);
    return TRUE;
}

int wmain(int argc, WCHAR* argv[])
{
    timeBeginPeriod(1);
    srand(unsigned int(time(nullptr)));
    

    return 0;
}