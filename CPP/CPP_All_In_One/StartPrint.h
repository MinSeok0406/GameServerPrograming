#pragma once
class IBaseObject
{
public:
    IBaseObject(int x, int y, bool flag) : _x(x), _y(y), _release(flag)
    {

    }

    virtual ~IBaseObject()
    {

    }

    virtual void Update() = 0;
    virtual void Render() = 0;

public:
    int _x;
    int _y;
    bool _release;
};

class OneStar : public IBaseObject
{
public:
    OneStar(int y) : IBaseObject(0, y, false)
    { }
    ~OneStar() { }

    void Update() override;
    void Render() override;
};

class TwoStar : public IBaseObject
{
public:
    TwoStar(int y) : IBaseObject(0, y, false)
    {}
    ~TwoStar() {}

    void Update() override;
    void Render() override;
};

class ThreeStar : public IBaseObject
{
public:
    ThreeStar(int y) : IBaseObject(0, y, false)
    {}
    ~ThreeStar() {}

    void Update() override;
    void Render() override;
};