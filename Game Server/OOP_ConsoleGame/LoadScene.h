#pragma once
#include "ISceneBase.h"

#define LENGTH 1024

class LoadScene : public ISceneBase
{
public:
    LoadScene();
    ~LoadScene();

    bool Update() override;

    void EnemyInit();
};