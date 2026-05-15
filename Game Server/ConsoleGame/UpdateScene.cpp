#include <Windows.h>
#include "UpdateScene.h"

void UpdateTitle()
{
    if (GetAsyncKeyState(VK_SPACE))
    {
        g_state = 1;
        g_scene = SCENE::LOAD;
    }
}

void UpdateLoad()
{
    // 스테이지에 맞는 정보 읽기
    // fopen_s, fread, fwrite 함수로 파일 정보 가져오기
    
    g_scene = SCENE::GAME;
}

void UpdateGame()
{

}

void UpdateClear()
{

}

void UpdateOver()
{

}