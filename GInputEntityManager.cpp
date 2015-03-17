#include "GGameDemoHeader.h"
#include "GInputEntityManager.h"
#include "GGameDemoHeader.h"
#include "GNode.h"


GInputEntityManager::GInputEntityManager ( void )
{
}


GInputEntityManager::~GInputEntityManager ( void )
{
}

void GInputEntityManager::addInputObj ( GNode* node )
{
    CXASSERT ( node );
    mInputObjIDList.push_back ( node->getLocalID() );
}

void GInputEntityManager::getInput ( u32 frameTimeMS )
{
for ( auto i : mInputObjIDList )
    {
        auto node = GNode::getObjIDManager().getObj ( i );
        if ( node )
            node->getInput ( frameTimeMS );
        else
            mFreeIDList.push_back ( i );
    }
for ( auto i: mFreeIDList )
    {
        mInputObjIDList.remove ( i );
    }
    mFreeIDList.clear();
}

void GInputEntityManager::remove ( u32 objID )
{
    mInputObjIDList.remove ( objID );
}
