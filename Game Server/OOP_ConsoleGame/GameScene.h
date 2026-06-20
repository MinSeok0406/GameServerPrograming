#pragma once
#include "ISceneBase.h"

class GameScene : public ISceneBase
{
public:
    GameScene();
    ~GameScene();

    bool Update() override;
};