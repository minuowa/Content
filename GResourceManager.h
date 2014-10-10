#pragma once
#include "XMap.h"
template<typename T>
class GResourceManager
{
public:
    typedef CXMap<GString, T*> ResourceMap;
public:
    //GResourceManagerTemplete ( void );
    virtual ~GResourceManager ( void )
    {
        dSafeDeleteMap2 ( mResourceMap );
    }
public:
    T* getResource ( const char* name )
    {
        T* res = 0;
        if ( !mResourceMap.Get ( name, res ) )
        {
            res = new T();
            if ( !res->createFromFile ( name ) )
            {
                CXASSERT ( 0 );
                dSafeDelete ( res );
            }
            else
            {
                mResourceMap.Insert ( name, res );
            }
        }
        return res;
    }
private:
    ResourceMap	mResourceMap;
};

