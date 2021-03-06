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

void GComponentMesh::onPropertyChange ( void* pre, void* changed )
{
	if ( pre == &mMeshFile )
	{
		uString changedName;
		dCast ( changedName, changed );
		if ( changedName != mMeshFile && mOwner )
		{
			mMeshFile = changedName;
			bool res = mOwner->recreate();
			CXASSERT ( res );
		}
	}
}
