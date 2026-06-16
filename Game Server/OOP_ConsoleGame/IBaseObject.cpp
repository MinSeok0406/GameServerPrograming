#include "IBaseObject.h"

IBaseObject::IBaseObject(int x, int y, OBJECT_TYPE obt)
    : _x(x), _y(y), _objectType(obt)
{

}

IBaseObject::~IBaseObject()
{

}

OBJECT_TYPE IBaseObject::GetObjectType()
{
    return _objectType;
}