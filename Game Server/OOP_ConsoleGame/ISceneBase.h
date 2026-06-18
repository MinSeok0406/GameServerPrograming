#pragma once

enum class SCENE
{
    TITLE,
    LOAD,
    GAME,
    CLEAR,
    OVER
};

class ISceneBase
{
public:
    ISceneBase() = default;
    ~ISceneBase() = default;

    virtual bool Update() = 0;
protected:
};