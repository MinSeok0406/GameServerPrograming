#include "IBaseObject.h"

IBaseObject::IBaseObject(OBJECT_TYPE objType, int x, int y, bool live)
    : _objectType(objType), _x(x), _y(y), _live(live)
{

}

IBaseObject::~IBaseObject()
{

}

OBJECT_TYPE IBaseObject::GetObjectType()
{
    return _objectType;
}