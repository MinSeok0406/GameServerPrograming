#include "Profile.h"

LARGE_INTEGER freq;
PROFILE_SAMPLE g_profile[50];

// 하나의 함수 Profiling 시작.
void ProfileBegin(TCHAR* szName)
{
    int cnt = -1;
    for (auto i = 0; i < 50; ++i)
    {
        // 문자열이 같은 경우
        if (!_tcscmp(g_profile[i].szName, szName))
        {
            g_profile[i].iCall++;
            QueryPerformanceCounter(&g_profile[cnt].lStartTime);
            return;
        }
    }

    for (auto i = 0; i < 50; ++i)
    {
        if (g_profile[i].lFlag == false)
        {
            cnt = i;
            break;
        }
    }
    
    g_profile[cnt].lFlag = true;
    _tcscpy_s(g_profile[cnt].szName, sizeof(g_profile[cnt].szName), szName);
    QueryPerformanceCounter(&g_profile[cnt].lStartTime);
    g_profile[cnt].iCall = 1;
}

// 하나의 함수 Profiling 끝 함수.
void ProfileEnd(TCHAR* szName)
{
    QueryPerformanceFrequency(&freq);



}

// Profiling 된 데이터를 Text 파일로 출력한다.
void ProfileDataOutText(TCHAR* szFileName)
{

}

// 프로파일링 된 데이터를 모두 초기화 한다.
void ProfileReset(void)
{

}
