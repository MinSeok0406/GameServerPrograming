#pragma once

#define LENGTH 1024
#define MAXSTAGE 5

enum class SCENE
{
    TITLE,
    LOAD,
    GAME,
    CLEAR,
    OVER
};

extern SCENE g_scene;
extern int g_stage;

// 프레임 표시
void FPS();

// 타이틀 로직 업데이트
void UpdateTitle();

// 로드 로직 업데이트
void UpdateLoad();

// 게임 로직 업데이트
void UpdateGame();

// 클리어 로직 업데이트
void UpdateClear();

// 게임 오버 로직 업데이트
void UpdateOver();