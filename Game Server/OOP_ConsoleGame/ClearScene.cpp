#include "ClearScene.h"
#include "Buffer.h"
#include "Console.h"
#include "SceneManager.h"
#include "ManagerObject.h"
#include <Windows.h>

extern Console* g_console;
extern ScreenBuffer* g_screenBuffer;
extern SceneManager* g_sceneManager;
extern ManagerObject* g_managerObject;
extern int g_stageBufferOffset;

ClearScene::ClearScene()
{}

ClearScene::~ClearScene()
{}

bool ClearScene::Update()
{
    g_screenBuffer->Buffer_Clear();
    g_screenBuffer->Sprite_Draw(20, 0, 'G');
    g_screenBuffer->Sprite_Draw(22, 0, 'A');
    g_screenBuffer->Sprite_Draw(24, 0, 'M');
    g_screenBuffer->Sprite_Draw(26, 0, 'E');
    g_screenBuffer->Sprite_Draw(30, 0, 'C');
    g_screenBuffer->Sprite_Draw(32, 0, 'L');
    g_screenBuffer->Sprite_Draw(34, 0, 'E');
    g_screenBuffer->Sprite_Draw(36, 0, 'A');
    g_screenBuffer->Sprite_Draw(38, 0, 'R');

    g_screenBuffer->Sprite_Draw(20, 2, 'G');
    g_screenBuffer->Sprite_Draw(22, 2, 'O');
    g_screenBuffer->Sprite_Draw(26, 2, 'T');
    g_screenBuffer->Sprite_Draw(28, 2, 'I');
    g_screenBuffer->Sprite_Draw(30, 2, 'T');
    g_screenBuffer->Sprite_Draw(32, 2, 'L');
    g_screenBuffer->Sprite_Draw(34, 2, 'E');
    g_screenBuffer->Sprite_Draw(36, 2, '?');
    g_screenBuffer->Sprite_Draw(38, 2, '?');

    g_screenBuffer->Sprite_Draw(18, 4, 'P');
    g_screenBuffer->Sprite_Draw(20, 4, 'R');
    g_screenBuffer->Sprite_Draw(22, 4, 'E');
    g_screenBuffer->Sprite_Draw(24, 4, 'E');
    g_screenBuffer->Sprite_Draw(26, 4, 'S');
    g_screenBuffer->Sprite_Draw(30, 4, 'T');
    g_screenBuffer->Sprite_Draw(32, 4, 'A');
    g_screenBuffer->Sprite_Draw(34, 4, 'B');
    g_screenBuffer->Buffer_Flip();

    if (GetAsyncKeyState(VK_TAB))
    {
        g_sceneManager->loadScene(SCENE::TITLE);
        g_stageBufferOffset = 0;
        g_console->cs_ClearScreen();
    }

    return true;
}
