#include "GGameDemoHeader.h"
#include "GComponentFactory.h"


GComponentFactory::ComponentCreator* GComponentFactory::getCreator ( const char* typeName ) const
{
    ComponentCreator* creator = 0;
    CXASSERT_RETURN_FALSE ( mComponentCreatorMap.Get ( typeName, creator ) );
    return creator;
}

GComponentFactory::ComponentCreator* GComponentFactory::getCreator ( eComponentType type ) const
{
    for ( ComponentCreatorMap::const_iterator walk = mComponentCreatorMap.begin();
            walk != mComponentCreatorMap.end(); ++walk )
    {
        ComponentCreator* componentCreator = walk->second;
        if ( componentCreator->mType == type )
        {
            return componentCreator;
        }
    }
    return 0;
}

void GComponentFactory::registerCreator ( const char* typeName, int type, ObjCrator creator )
{
    if ( mComponentCreatorMap.find ( typeName ) == mComponentCreatorMap.end() )
    {
        ComponentCreator* componentCreator = new ComponentCreator;
        componentCreator->mType = type;
        componentCreator->mCreator = creator;
        mComponentCreatorMap.Insert ( typeName, componentCreator );
    }
    else
    {
        CXASSERT ( 0 );
    }
}

GComponentInterface* GComponentFactory::create ( const char* typeName )
{
    ComponentCreator* creator = getCreator ( typeName );
    CXASSERT_RETURN_FALSE ( creator );
    return creator->mCreator();
}

GComponentFactory::~GComponentFactory()
{
	dSafeDeleteMap2(mComponentCreatorMap);
}
