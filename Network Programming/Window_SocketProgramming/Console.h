#pragma once

#define dfSCREEN_WIDTH	81
#define dfSCREEN_HEIGHT	23

// 콘솔 제어 준비 작업
void cs_Initial(void);

// 콘솔 화면의 커서를 X, Y 좌표로 이동
void cs_MoveCursor(int iPosX, int iPosY);

// 콘솔 화면을 초기화
void cs_ClearScreen(void);

BOOL DomainToIP(const WCHAR* szDomain, IN_ADDR* pAddr)
{
    ADDRINFOW* pAddrInfo;
    SOCKADDR_IN* pSockAddr[1024];

    if (GetAddrInfo(szDomain, L"0", NULL, &pAddrInfo) != 0)
    {
        auto errCode = WSAGetLastError();
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