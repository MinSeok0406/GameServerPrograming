#pragma once

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
extern int tick;

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