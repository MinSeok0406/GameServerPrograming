#include "TitleScene.h"
#include "Buffer.h"
#include "Console.h"
#include "SceneManager.h"
#include <Windows.h>

extern SceneManager* g_sceneManager;
extern ScreenBuffer* g_screenBuffer;
extern Console* g_console;
extern int g_stage;

TitleScene::TitleScene()
{}

TitleScene::~TitleScene()
{}

bool TitleScene::Update()
{
    g_screenBuffer->Buffer_Clear();
    g_screenBuffer->Sprite_Draw(20, 0, 'G');
    g_screenBuffer->Sprite_Draw(22, 0, 'A');
    g_screenBuffer->Sprite_Draw(24, 0, 'M');
    g_screenBuffer->Sprite_Draw(26, 0, 'E');
    g_screenBuffer->Sprite_Draw(30, 0, 'R');
    g_screenBuffer->Sprite_Draw(32, 0, 'E');
    g_screenBuffer->Sprite_Draw(34, 0, 'A');
    g_screenBuffer->Sprite_Draw(36, 0, 'D');
    g_screenBuffer->Sprite_Draw(38, 0, 'Y');

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
        g_stage = 1;
        g_sceneManager->loadScene(SCENE::LOAD);
        g_console->cs_ClearScreen();
    }

    return true;
}
