#pragma once
#include "XString.h"
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

class GObject: public CXCallBack
{
	DeclareCategoryName ( GObject );
public:
	GObject ( void );
	virtual ~GObject ( void );
public:
	template<typename T>
	void registerProperty ( const char* categoryName, const char* propName, const T& var );
	template<>
	void registerProperty ( const char* categoryName, const char* propName, const GString& var );
	template<typename T>
	void registerEnumProperty ( const char* categoryName, const char* propName, const T& var );
	void registerAll();
	void unRegisterAll();
	void setProperty(const char* categoryName,const char* propName,const char* var);
	void registerProperty ( GObject* obj );
	const CategoryPropertyMap& getPropertyMap() const;
	CategoryPropertyMap& getPropertyMap();
	virtual void onPropertyChange ( void* pre, void* changed );
	virtual void onPropertyChangeEnd ( void* cur );
protected:
	virtual void registerAllProperty();
	CategoryPropertyMap mOption;
	GString	mNodeName;
public:
	static GString mTargetName;
	static GString mOperatorObjectName;
	static CXDelegate mDelegateAlterName;

	bool isRegist();
	const char* getObjectName() const;
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
void GObject::registerProperty ( const char* categoryName, const char* propName, const GString& var )
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
		evar->mProp = new CXPropEntity<GString> ( ( GString* ) &var, false );
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
