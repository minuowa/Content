#include "GGameDemoHeader.h"
#include "GObject.h"

int gCount = 0;
GObject::GObject ( void )
{
	mNodeName.clear();
	mNodeName.Format ( "GObject%d", gCount );

	gCount++;
}


GObject::~GObject ( void )
{
	unRegisterAll();
}

void GObject::registerAllProperty()
{
	registerProperty ( "GObject", "mNodeName", mNodeName );
	//__RegisterProperty(mEObjectName);
}

void GObject::registerProperty ( GObject* obj )
{
	assert ( obj );
	obj->registerAll();

	const CategoryPropertyMap& otherPropMap = obj->getPropertyMap();

	CategoryPropertyMap::const_iterator ibegin = otherPropMap.begin();
	CategoryPropertyMap::const_iterator iend = otherPropMap.end();

	for ( ; ibegin != iend; ++ibegin )
	{
		PropertyMap* myPropMap = 0;
		if ( mOption.Get ( ibegin->first, myPropMap ) )
		{
			assert ( 0 );
		}
		else
		{
			myPropMap = new PropertyMap;
			mOption.Insert ( ibegin->first, myPropMap );
		}
		const PropertyMap* propMap = ibegin->second;
		for ( PropertyMap::const_iterator walk = propMap->begin();
			walk != propMap->end(); ++walk )
		{
			EPropertyVar* evar = new EPropertyVar;
			evar->mPtr = walk->second->mPtr;
			evar->mCategoryName = walk->second->mCategoryName;
			evar->mProp = walk->second->mProp;
			evar->mRefOther = true;
			myPropMap->Insert ( walk->first, evar );
		}
	}
	//mOption.insert ( ibegin, iend );
}

void GObject::unRegisterAll()
{
	CategoryPropertyMap::iterator walk = mOption.begin();
	CategoryPropertyMap::iterator end = mOption.end();
	for ( ; walk != end; ++walk )
	{
		PropertyMap* propMap = walk->second;
		dSafeDeleteMap2 ( *propMap );
	}
	dSafeDeleteMap2 ( mOption );
}

void GObject::onPropertyChange ( void* pre, void* changed )
{
	if ( pre == &mNodeName )
	{
		GString changedName;
		dCast ( changedName, changed );
		mTargetName = changedName;
		mOperatorObjectName = mNodeName;
		mDelegateAlterName.trigger();
	}
}

void GObject::registerAll()
{
	unRegisterAll();
	registerAllProperty();
}

void GObject::onPropertyChangeEnd ( void* cur )
{

}

const CategoryPropertyMap& GObject::getPropertyMap() const
{
	return mOption;
}

CategoryPropertyMap& GObject::getPropertyMap()
{
	return mOption;
}

bool GObject::isRegist()
{
	return !mOption.empty();
}

const char* GObject::getObjectName() const
{
	return mNodeName.c_str();
}

void GObject::setProperty ( const char* categoryName, const char* propName, const char* var )
{
	PropertyMap* propMap = 0;
	CXASSERT_RETURN ( mOption.Get ( categoryName, propMap ) );

	EPropertyVar* evar = 0;
	CXASSERT_RETURN ( propMap->Get ( propName, evar )  );
	evar->mProp->setValue ( var );
}

GString GObject::mTargetName;

GString GObject::mOperatorObjectName;

CXDelegate GObject::mDelegateAlterName;

EPropertyVar::EPropertyVar()
	: mPtr ( 0 )
	, mRefOther ( false )
{

}
void EPropertyVar::ToString ( std::string& str )
{
	if ( mProp )
		mProp->toString ( str );
}

EPropertyVar::~EPropertyVar()
{
	if ( !mRefOther )
		dSafeDelete ( mProp );
}
