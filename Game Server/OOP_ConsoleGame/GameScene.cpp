#include "GameScene.h"
#include "Buffer.h"
#include "SceneManager.h"
#include "ManagerObject.h"
#include "Console.h"
#include <Windows.h>

extern Console* g_console;
extern ScreenBuffer* g_screenBuffer;
extern SceneManager* g_sceneManager;
extern ManagerObject* g_managerObject;
extern int g_stage;
int tick;

GameScene::GameScene()
{}

GameScene::~GameScene()
{}

// 10fps 게임으로 만든다.
bool GameScene::Update()
{
    bool flag = true;

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

    // 적이 필드에 있는지 검사
    for (auto& objList : g_managerObject->_objectList)
    {
        if ((objList->GetObjectType() == OBJECT_TYPE::ENEMY) &&
            (objList->GetLive() == true))
        {
            flag = false;
            break;
        }
    }

    if (flag)
    {
        g_sceneManager->loadScene(SCENE::LOAD);
        g_stage++;
        g_console->cs_ClearScreen();
        return true;
    }

    // 문제 발생하면 메인문으로 빼기
    int useTime = (int)(timeGetTime() - tick);
    if (useTime > 0 && useTime < 100)
    {
        Sleep(100 - useTime);
    }

    return true;
}
