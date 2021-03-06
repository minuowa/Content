#pragma once
#include "uString.h"
#include "XMap.h"
#include "XMultimap.h"
#include "XProp.h"
#include "XDelegate.h"


#define DeclareCategoryName(className)\
		public:\
		virtual const char* categoryName() const\
{\
	return #className;\
}\
 
struct  EPropertyVar
{
    uString mCategoryName;
    void* mPtr;
    CXProp* mProp;
    bool mRefOther;

    void ToString ( std::string& str );
    EPropertyVar();
    ~EPropertyVar();
};
typedef CXMap<uString, EPropertyVar*> PropertyMap;
typedef CXMap<uString, PropertyMap*> CategoryPropertyMap;
struct CXAlterNameArgs:public CXEventArgs 
{
	uString mCurName;
	uString mChangedName;
};
class GObject: public CXCallBack
{
    DeclareCategoryName ( GObject );
    DeclareFilmObj ( GObject );
public:
    GObject ( void );
    virtual ~GObject ( void );
public:
    DeclareFilmTool void setName ( const char* name );

    template<typename T>
    void registerProperty ( const char* categoryName, const char* propName, const T& var );
    template<>
    void registerProperty ( const char* categoryName, const char* propName, const uString& var );
    template<typename T>
    void registerEnumProperty ( const char* categoryName, const char* propName, const T& var );
    void registerAll();
    void unRegisterAll();
    void setProperty ( const char* categoryName, const char* propName, const char* var );
    void registerProperty ( GObject* obj );
    const CategoryPropertyMap& getPropertyMap() const;
    CategoryPropertyMap& getPropertyMap();
    virtual void onPropertyChange ( void* pre, void* changed );
    virtual void onPropertyChangeEnd ( void* cur );
protected:
    virtual void registerAllProperty();
    CategoryPropertyMap mOption;
    uString	mName;
public:

    static CXDelegate mDelegateAlterName;

    bool isRegist();
    const char* getName() const;
};


template<typename T>
void GObject::registerEnumProperty ( const char* categoryName, const char* propName, const T& var )
{
    PropertyMap* propMap = 0;
    if ( !mOption.Get ( categoryName, propMap ) )
    {
        propMap = new PropertyMap;
        mOption.Insert ( categoryName, propMap );
    }

    EPropertyVar* evar = 0;
    if ( !propMap->Get ( propName, evar ) )
    {
        evar = new EPropertyVar;
        //if ( CXEnumStructHelper<T>::mStructList.empty() )
        //    CXEnumStructHelper<T>::regist();
        CXPropEnum* penum = new CXPropEnum ( ( int* ) &var, CXEnumStructHelper<T>::getStructList(), false );
        evar->mProp = penum;
        evar->mPtr = ( void* ) &var;
        evar->mCategoryName = categoryName;
        propMap->Insert ( propName, evar );
    }
    else
    {
        assert ( 0 && "already exist property" );
    }
}

template<typename T>
void GObject::registerProperty ( const char* categoryName, const char* propName, const T& var )
{
    PropertyMap* propMap = 0;
    if ( !mOption.Get ( categoryName, propMap ) )
    {
        propMap = new PropertyMap;
        mOption.Insert ( categoryName, propMap );
    }

    EPropertyVar* evar = 0;
    if ( !propMap->Get ( propName, evar ) )
    {
        evar = new EPropertyVar;
        evar->mProp = new CXPropEntity<T> ( ( T* ) &var, false );
        evar->mPtr = ( void* ) &var;
        evar->mCategoryName = categoryName;
        propMap->Insert ( propName, evar );
    }
    else
    {
        assert ( 0 && "already exist property" );
    }
}

template<>
void GObject::registerProperty ( const char* categoryName, const char* propName, const uString& var )
{
    PropertyMap* propMap = 0;
    if ( !mOption.Get ( categoryName, propMap ) )
    {
        propMap = new PropertyMap;
        mOption.Insert ( categoryName, propMap );
    }

    EPropertyVar* evar = 0;
    if ( !propMap->Get ( propName, evar ) )
    {
        evar = new EPropertyVar;
        evar->mCategoryName = categoryName;
        //evar->mProp = new CXPropEntity<String> ( new  String(var.c_str() ),false);
        evar->mProp = new CXPropEntity<uString> ( ( uString* ) &var, false );
        evar->mPtr = ( void* ) &var;
        propMap->Insert ( propName, evar );
    }
    else
    {
        assert ( 0 && "already exist property" );
    }
}

#define __RegisterProperty(mem) registerProperty(categoryName(),#mem,mem)
#define __RegisterPropertyEnum(mem) registerEnumProperty(categoryName(),#mem,mem)
