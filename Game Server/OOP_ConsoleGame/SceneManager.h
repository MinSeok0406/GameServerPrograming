#pragma once
#include "ISceneBase.h"

class SceneManager
{
public:
    static SceneManager* getInstance()
    {
        if (_pSceneManager == nullptr)
        {
            _pSceneManager = new SceneManager();
        }

        return _pSceneManager;
    }

    void destoryInstance()
    {
        if (_pSceneManager != nullptr)
        {
            delete _pSceneManager;
            _pSceneManager = nullptr;
        }
    }

    void run();
    void loadScene(SCENE sceneType);

    ISceneBase* _pScene;

private:
    static SceneManager* _pSceneManager;
    SceneManager() = default;
    ~SceneManager() = default;
};