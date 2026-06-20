#pragma once
#include "ISceneBase.h"

class OverScene : public ISceneBase
{
public:
    OverScene();
    ~OverScene();

    bool Update() override;
};