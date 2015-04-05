#include "GGameDemoHeader.h"
#include "GGWireRectNode.h"


GGWireRectNode::GGWireRectNode ( void )
{
    mGraph.mType = D3DPT_LINELIST;
    mGraph.mPrimitiveCount = 4;
}


GGWireRectNode::~GGWireRectNode ( void )
{
}

bool GGWireRectNode::recreate()
{
    if ( !mVB.isValid() )
    {
        mVB.mElementSize = sizeof ( GRectVertex );
        mVB.mFVF = GRectVertex::mFVF;
        mVB.mElementCount = 4;
        CXASSERT_RETURN_FALSE ( mVB.recreate ( D3DPOOL_MANAGED ) );
    }

    CXASSERT_RETURN_FALSE ( doGraph() );

    if ( !mIB.isValid() )
    {
        mIB.mIndexCount = 8;
        CXASSERT_RETURN_FALSE ( mIB.recreate ( D3DPOOL_MANAGED ) );
    }
    DWORD aIndex[8] =
    {
        0, 1, 1, 2, 2, 3, 3, 0,
    };

    CXASSERT_RETURN_FALSE ( mIB.fill ( ( void* ) aIndex ) );

    return true;
}

bool GGWireRectNode::doGraph()
{
	return GRectNode::doGraph();
}

bool GGWireRectNode::render()
{
	return GRectNode::render();
}
