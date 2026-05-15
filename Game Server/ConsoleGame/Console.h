#pragma once

#define dfSCREEN_WIDTH	81
#define dfSCREEN_HEIGHT	24

// 콘솔 제어 준비 작업
void cs_Initial(void);

// 콘솔 화면의 커서를 X, Y 좌표로 이동
void cs_MoveCursor(int iPosX, int iPosY);

// 콘솔 화면을 초기화
void cs_ClearScreen(void);