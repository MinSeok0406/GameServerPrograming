#pragma once

enum class OBJECT_TYPE
{
    PLAYER,
    ENEMY,
    PLAYER_BULLET,
    ENEMY_BULLET
};

class IBaseObject
{
public:
    IBaseObject(int x, int y, OBJECT_TYPE obt);
    virtual ~IBaseObject();

    virtual bool Update() = 0;
    virtual void Render() = 0;

    OBJECT_TYPE GetObjectType();

protected:
    int _x;
    int _y;
    OBJECT_TYPE _objectType;
};