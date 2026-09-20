#include "ObjectFreeList.h"

unsigned __int64 getObjectSize(unsigned __int64 objectsize)
{
    if (objectsize <= (unsigned __int64)OBJECTSIZE::B8)
    {
        return (unsigned __int64)OBJECTSIZE::B8;
    }
    else if (objectsize <= (unsigned __int64)OBJECTSIZE::B16)
    {
        return (int)OBJECTSIZE::B16;
    }
    else if (objectsize <= (unsigned __int64)OBJECTSIZE::B32)
    {
        return (unsigned __int64)OBJECTSIZE::B32;
    }
    else if (objectsize <= (unsigned __int64)OBJECTSIZE::B64)
    {
        return (unsigned __int64)OBJECTSIZE::B64;
    }
    else if (objectsize <= (unsigned __int64)OBJECTSIZE::B128)
    {
        return (unsigned __int64)OBJECTSIZE::B128;
    }
    else if (objectsize <= (unsigned __int64)OBJECTSIZE::B256)
    {
        return (unsigned __int64)OBJECTSIZE::B256;
    }
    else if (objectsize <= (unsigned __int64)OBJECTSIZE::B512)
    {
        return (unsigned __int64)OBJECTSIZE::B512;
    }
    else if (objectsize <= (unsigned __int64)OBJECTSIZE::KB1)
    {
        return (unsigned __int64)OBJECTSIZE::KB1;
    }
    else if (objectsize <= (unsigned __int64)OBJECTSIZE::KB2)
    {
        return (unsigned __int64)OBJECTSIZE::KB2;
    }
    else if (objectsize <= (unsigned __int64)OBJECTSIZE::KB4)
    {
        return (unsigned __int64)OBJECTSIZE::KB4;
    }
    else if (objectsize <= (unsigned __int64)OBJECTSIZE::KB8)
    {
        return (unsigned __int64)OBJECTSIZE::KB8;
    }
    else if (objectsize <= (unsigned __int64)OBJECTSIZE::KB16)
    {
        return (unsigned __int64)OBJECTSIZE::KB16;
    }
    else if (objectsize <= (unsigned __int64)OBJECTSIZE::KB32)
    {
        return (unsigned __int64)OBJECTSIZE::KB32;
    }
    else if (objectsize <= (unsigned __int64)OBJECTSIZE::KB64)
    {
        return (unsigned __int64)OBJECTSIZE::KB64;
    }
    else if (objectsize <= (unsigned __int64)OBJECTSIZE::KB128)
    {
        return (unsigned __int64)OBJECTSIZE::KB128;
    }
    else if (objectsize <= (unsigned __int64)OBJECTSIZE::KB256)
    {
        return (unsigned __int64)OBJECTSIZE::KB256;
    }
    else if (objectsize <= (unsigned __int64)OBJECTSIZE::KB512)
    {
        return (unsigned __int64)OBJECTSIZE::KB512;
    }
    else if (objectsize <= (unsigned __int64)OBJECTSIZE::MB1)
    {
        return (unsigned __int64)OBJECTSIZE::MB1;
    }
}