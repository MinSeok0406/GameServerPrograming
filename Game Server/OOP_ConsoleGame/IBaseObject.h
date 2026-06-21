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
    IBaseObject(OBJECT_TYPE objType, int x, int y, bool live);
    virtual ~IBaseObject();

    virtual bool Update() = 0;
    virtual void Render() = 0;
    virtual bool RemoveObject() = 0;
    virtual bool OnCollision(IBaseObject* obj) = 0;

    OBJECT_TYPE GetObjectType();

    // 개선이 필요한 코드
    int GetX() { return _x; }
    int GetY() { return _y; }
    bool GetLive() { return _live; }

protected:
    bool _live;
    int _x;
    int _y;
    OBJECT_TYPE _objectType;
};