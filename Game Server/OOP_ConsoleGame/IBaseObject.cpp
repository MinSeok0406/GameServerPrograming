#include "IBaseObject.h"

IBaseObject::IBaseObject(OBJECT_TYPE objType)
    : _objectType(objType)
{

}

IBaseObject::~IBaseObject()
{

}

OBJECT_TYPE IBaseObject::GetObjectType()
{
    return _objectType;
}