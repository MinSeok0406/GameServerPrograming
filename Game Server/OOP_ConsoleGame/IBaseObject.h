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
    IBaseObject(OBJECT_TYPE objType);
    virtual ~IBaseObject();

    virtual bool Update() = 0;
    virtual void Render() = 0;
    virtual bool RemoveObject() = 0;
    virtual bool OnCollision(IBaseObject* obj) = 0;

    OBJECT_TYPE GetObjectType();

protected:
    OBJECT_TYPE _objectType;
};