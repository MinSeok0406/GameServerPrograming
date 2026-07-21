#pragma once

// 버퍼의 내용을 화면으로 찍어주는 함수
void Buffer_Flip(void);

// 화면 버퍼를 지우는 함수
void Buffer_Clear(void);

// 버퍼의 특정 위치에 원하는 문자를 출력
void Sprite_Draw(int iX, int iY, wchar_t chSprite);