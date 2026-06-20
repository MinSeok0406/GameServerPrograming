#pragma once
#include "ISceneBase.h"

class ClearScene : public ISceneBase
{
public:
    ClearScene();
    ~ClearScene();

    bool Update() override;
};