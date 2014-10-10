#include "GGameDemoHeader.h"
#include "GComponentMesh.h"
#include "GNode.h"


GComponentMesh::GComponentMesh ( void )
{
}


GComponentMesh::~GComponentMesh ( void )
{
}

void GComponentMesh::registerAllProperty()
{
    __RegisterProperty ( mMeshFile );
}

void GComponentMesh::OnPropertyChange ( void* pre, void* changed )
{
    if ( pre == &mMeshFile )
    {
        GString changedName;
        dCast ( changedName, changed );
        if ( changedName != mMeshFile && mTarget )
        {
            mMeshFile = changedName;
            bool res = mTarget->reCreate();
            CXASSERT ( res );
        }
    }
}
