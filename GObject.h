#pragma once
#include "XString.h"
#include "XMap.h"
#include "XMultimap.h"
#include "XProp.h"
#include "XDelegate.h"

typedef GString GString;

#define DeclareCategoryName(className)\
		public:\
		virtual const char* categoryName() const\
{\
	return #className;\
}\
 
struct  EPropertyVar
{
	GString mCategoryName;
	void* mPtr;
    CXProp* mProp;
	bool mRefOther;

    void ToString ( std::string& str );
    EPropertyVar();
	~EPropertyVar();
};
typedef CXMap<GString, EPropertyVar*> PropertyMap;
typedef CXMap<GString, PropertyMap*> CategoryPropertyMap;

class GObject:public CXCallBack
{
    DeclareCategoryName ( GObject );
public:
    GObject ( void );
    virtual ~GObject ( void );
public:
    template<typename T>
    void registerProperty ( const char* categoryName, const char* propName, const T& var )
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
            evar->mProp = new CXPropEntity<T> ( (T*)&var,false);
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
    void registerProperty ( const char* categoryName, const char* propName, const GString& var )
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
			evar->mProp = new CXPropEntity<GString> ( (GString*)&var,false);
            evar->mPtr = ( void* ) &var;
            propMap->Insert ( propName, evar );
        }
        else
        {
            assert ( 0 && "already exist property" );
        }
    }
	void registerAll();
	void unRegisterAll();
    void registerProperty ( GObject* obj );
    const CategoryPropertyMap& getPropertyMap() const
    {
        return mOption;
    }
    CategoryPropertyMap& getPropertyMap()
    {
        return mOption;
    }
	virtual void onPropertyChange(void* pre,void* changed);
protected:
	virtual void registerAllProperty();
	GString	mNodeName;
    CategoryPropertyMap mOption;
public:
	static GString mTargetName;
	static GString mOperatorObjectName;
	static CXDelegate mDelegateAlterName;

    bool isRegist()
    {
        return !mOption.empty();
    }

	inline CChar* getObjectName() const
	{
		return mNodeName.c_str();
	}
};


#define __RegisterProperty(mem) registerProperty(categoryName(),#mem,mem)
