#include "GameScene.h"
#include "Buffer.h"
#include "SceneManager.h"
#include "ManagerObject.h"
#include <Windows.h>

extern ScreenBuffer* g_screenBuffer;
extern SceneManager* g_sceneManager;
extern ManagerObject* g_managerObject;
int tick;

GameScene::GameScene()
{}

GameScene::~GameScene()
{}

// 10fps 게임으로 만든다.
bool GameScene::Update()
{
    // 문제 발생하면 메인문으로 빼기
    tick = timeGetTime();

    // 로직
    g_managerObject->Update();

    // 렌더링
    g_screenBuffer->Buffer_Clear();
    g_managerObject->Render();
    g_screenBuffer->Buffer_Flip();

    // 객체 삭제 관련 함수
    g_managerObject->RemoveObject();

    // 문제 발생하면 메인문으로 빼기
    int useTime = (int)(timeGetTime() - tick);
    if (useTime > 0 && useTime < 100)
    {
        Sleep(100 - useTime);
    }

    return true;
}
