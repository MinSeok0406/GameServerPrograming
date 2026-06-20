#include "IBaseObject.h"

IBaseObject::IBaseObject(OBJECT_TYPE objType, int x, int y)
    : _objectType(objType), _x(x), _y(y)
{

}

IBaseObject::~IBaseObject()
{

}

OBJECT_TYPE IBaseObject::GetObjectType()
{
    return _objectType;
}