#include "GGameDemoHeader.h"
#include "GComponentInterface.h"
#include "GComponentTrans.h"
#include "GFactory.h"
#include "GComponentFactory.h"
#include "GNode.h"


GComponentInterface::GComponentInterface ( void )
	: mComponentType ( eComponentType_Count )
	, mCanDetach ( true )
	, mAutoTargetRotation ( 0 )
{
}


GComponentInterface::~GComponentInterface ( void )
{
}

void GComponentInterface::SetTarget ( GNode* target )
{
	mAutoTargetRotation = target;
}

GNode* GComponentInterface::GetTarget() const
{
	return mAutoTargetRotation;
}




GComponentOwner::GComponentOwner()
{
	dMemoryZeroArray ( mCompoents );
}

GComponentInterface* GComponentOwner::getComponent ( const char* name ) const
{
	for ( int i = 0; i < eComponentType_Count; ++i )
	{
		if ( mCompoents[i] )
		{
			if ( !strcmp ( mCompoents[i]->categoryName(), name ) )
			{
				return mCompoents[i];
			}
		}
	}
	return 0;
}

GComponentInterface* GComponentOwner::getComponent ( eComponentType type ) const
{
	assert ( type < eComponentType_Count );
	return mCompoents[type];
}




GComponentInterface* GComponentOwner::attachComponent ( eComponentType type )
{
	assert ( type < eComponentType_Count );
	if ( !getComponent ( type ) )
	{
		GComponentFactory::ComponentCreator* creator =
			CXSingleton<GComponentFactory>::GetSingleton().getCreator ( type );
		CXASSERT_RESULT_FALSE ( creator );
		mCompoents[type] = creator->mCreator();
	}
	return mCompoents[type];
}

GComponentInterface* GComponentOwner::attachComponent ( const char* name )
{
	GComponentInterface* component = getComponent ( name );
	if ( component )
	{
		return component;
	}
	GComponentFactory::ComponentCreator* creator =
		CXSingleton<GComponentFactory>::GetSingleton().getCreator ( name );
	CXASSERT_RESULT_FALSE ( creator );
	CXASSERT_RESULT_FALSE ( !mCompoents[creator->mType] );
	mCompoents[creator->mType] = creator->mCreator();
	return mCompoents[creator->mType];
}


void GComponentOwner::detachComponent ( const char* name )
{
	for ( int i = 0; i < eComponentType_Count; ++i )
	{
		if ( mCompoents[i] )
		{
			if ( !strcmp ( mCompoents[i]->categoryName(), name ) )
			{
				if ( mCompoents[i]->canDetach() )
				{
					dSafeDelete ( mCompoents[i] );
				}
			}
		}
	}
}

GComponentOwner::~GComponentOwner()
{
	for ( GComponentInterface * component: mCompoents )
	{
		dSafeDelete ( component );
	}
}



