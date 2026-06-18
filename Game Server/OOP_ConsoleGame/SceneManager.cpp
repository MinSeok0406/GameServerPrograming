#include "SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "LoadScene.h"
#include "ClearScene.h"
#include "OverScene.h"

SceneManager* SceneManager::_pSceneManager = nullptr;

void SceneManager::run()
{
    _pScene->Update();
}

void SceneManager::loadScene(SCENE sceneType)
{
    delete _pScene;

	switch (sceneType)
	{
	case SCENE::TITLE:
		_pScene = new TitleScene;
		break;
	case SCENE::LOAD:
		_pScene = new LoadScene;
		break;
	case SCENE::GAME:
		_pScene = new GameScene;
		break;
	case SCENE::CLEAR:
		_pScene = new ClearScene;
		break;
	case SCENE::OVER:
		_pScene = new OverScene;
		break;
	}
}