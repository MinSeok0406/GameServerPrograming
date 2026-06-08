#define _CRT_SECURE_NO_WARNINGS
#include "Profile.h"
#include <cstdio>

LARGE_INTEGER freq;
PROFILE_SAMPLE g_profile[50];

// 하나의 함수 Profiling 시작.
void ProfileBegin(const TCHAR* szName)
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
    _tcscpy(g_profile[cnt].szName, szName);
    QueryPerformanceCounter(&g_profile[cnt].lStartTime);
    g_profile[cnt].iCall = 1;
    
    for (auto i = 0; i < 2; ++i)
    {
        g_profile[cnt].iMax[i] = (__int64)-1e9;
        g_profile[cnt].iMin[i] = (__int64)1e9;
    }
}

// 하나의 함수 Profiling 끝 함수.
void ProfileEnd(const TCHAR* szName)
{
    QueryPerformanceFrequency(&freq);

    for (auto i = 0; i < 50; ++i)
    {
        // 문자열이 같은 경우
        if (!_tcscmp(g_profile[i].szName, szName))
        {
            LARGE_INTEGER end;
            QueryPerformanceCounter(&end);
            double t = (end.QuadPart - g_profile[i].lStartTime.QuadPart) / freq.QuadPart;
            g_profile[i].iTotalTime += t;

            for (auto j = 0; j < 2; ++j)
            {
                if (g_profile[i].iTotalTime > g_profile[i].iMax[j])
                {
                    g_profile[i].iMax[j] = g_profile[i].iTotalTime;
                }

                if (g_profile[i].iTotalTime < g_profile[i].iMin[j])
                {
                    g_profile[i].iMin[j] = g_profile[i].iTotalTime;
                }
            }

            return;
        }
    }
}

// Profiling 된 데이터를 Text 파일로 출력한다.
void ProfileDataOutText(const TCHAR* szFileName)
{
    FILE* fp;
    _tfopen_s(&fp, szFileName, _T("wb"));
    if (fp == nullptr)
    {
        return;
    }

    fwprintf(fp, _T("--------------------------------------------------------------------\n\n"));
    fwprintf(fp, _T("\t Name| \t Average| \t Min| \t Max| \t Call|\n"));
    fwprintf(fp, _T("--------------------------------------------------------------------\n"));

    for (auto i = 0; i < 50; ++i)
    {
        if (g_profile[i].lFlag == false)
        {
            break;
        }

        fwprintf(fp, _T("\t %s| \t %lf| \t %lf| \t %lf| \t %lld|\n"),
            g_profile[i].szName, (double)g_profile[i].iTotalTime / g_profile[i].iCall, 
            g_profile[i].iMin[0], g_profile[i].iMax[0], g_profile[i].iCall);
    }

    fwprintf(fp, _T("--------------------------------------------------------------------\n"));
    fwprintf(fp, _T("--------------------------------------------------------------------\n\n"));

    fclose(fp);
}

// 프로파일링 된 데이터를 모두 초기화 한다.
void ProfileReset(void)
{

}