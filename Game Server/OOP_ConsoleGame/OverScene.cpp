#include "OverScene.h"
#include "Buffer.h"
#include "Console.h"
#include "SceneManager.h"
#include <Windows.h>

extern Console* g_console;
extern ScreenBuffer* g_screenBuffer;
extern SceneManager* g_sceneManager;

OverScene::OverScene()
{}

OverScene::~OverScene()
{}

bool OverScene::Update()
{
    g_screenBuffer->Buffer_Clear();
    g_screenBuffer->Sprite_Draw(20, 0, 'G');
    g_screenBuffer->Sprite_Draw(22, 0, 'A');
    g_screenBuffer->Sprite_Draw(24, 0, 'M');
    g_screenBuffer->Sprite_Draw(26, 0, 'E');
    g_screenBuffer->Sprite_Draw(30, 0, 'O');
    g_screenBuffer->Sprite_Draw(32, 0, 'V');
    g_screenBuffer->Sprite_Draw(34, 0, 'E');
    g_screenBuffer->Sprite_Draw(36, 0, 'R');

    g_screenBuffer->Sprite_Draw(18, 2, 'P');
    g_screenBuffer->Sprite_Draw(20, 2, 'R');
    g_screenBuffer->Sprite_Draw(22, 2, 'E');
    g_screenBuffer->Sprite_Draw(24, 2, 'E');
    g_screenBuffer->Sprite_Draw(26, 2, 'S');
    g_screenBuffer->Sprite_Draw(30, 2, 'T');
    g_screenBuffer->Sprite_Draw(32, 2, 'A');
    g_screenBuffer->Sprite_Draw(34, 2, 'B');
    g_screenBuffer->Buffer_Flip();

    if (GetAsyncKeyState(VK_TAB))
    {
        g_sceneManager->loadScene(SCENE::GAME);
        g_console->cs_ClearScreen();
    }

    return true;
}