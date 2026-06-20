#pragma once
#include "ISceneBase.h"

class TitleScene : public ISceneBase
{
public:
    TitleScene();
    ~TitleScene();

    bool Update() override;
};