#pragma once
#include <tchar.h>
#include <Windows.h>

#define PROFILE
#define INDEX			50
#define MICROSECOND		1000000

struct PROFILE_SAMPLE
{
	long			lFlag;				// 프로파일의 사용 여부. (배열시에만)
	WCHAR			szName[64];			// 프로파일 샘플 이름.

	LARGE_INTEGER	lStartTime;			// 프로파일 샘플 실행 시간.

	__int64			iTotalTime;			// 전체 사용시간 카운터 Time.	(출력시 호출회수로 나누어 평균 구함)
	__int64			iMin[2];			// 최소 사용시간 카운터 Time.	(초단위로 계산하여 저장 / [0] 가장최소 [1] 다음 최소 [2])
	__int64			iMax[2];			// 최대 사용시간 카운터 Time.	(초단위로 계산하여 저장 / [0] 가장최대 [1] 다음 최대 [2])

	__int64			iCall;				// 누적 호출 횟수.
};

// 하나의 함수 Profiling 시작, 끝 함수.
void ProfileBegin(const WCHAR* szName);
void ProfileEnd(const WCHAR* szName);

// Profiling 된 데이터를 Text 파일로 출력한다.
void ProfileDataOutText(const WCHAR* szFileName);

// 프로파일링 된 데이터를 모두 초기화 한다.
void ProfileReset(void);

__int64 FileTimeToQuadWord(PFILETIME pft);

// 쓰레드 별 커널 및 유저 시간 합
void performLongOperation();