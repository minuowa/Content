#include "GGameDemoHeader.h"
#include "GTapeNode.h"
#include "Content.h"

static const int Width = 8;

GTapeNode::GTapeNode ( void )
{
    setState ( eUINodeState_AcpectEvent, false );

    dMemoryZeroArray ( mCorner );

    for ( int i = 0; i < CornerCount; ++i )
    {
        mCorner[i] = new GRectNode();
        mCorner[i]->recreate();
        mCorner[i]->setWH ( Width, Width );
    }
}


GTapeNode::~GTapeNode ( void )
{
    dSafeDeleteArray ( mCorner );
}

bool GTapeNode::recreate()
{
    return GRectNode::recreate();
}

bool GTapeNode::render()
{
    if ( !GNode::render() )
        return false;
    Content::Device.renderFirstGraph ( false );
for ( GRectNode * n : mCorner )
        n->draw();
    Content::Device.renderFirstGraph ( true );
    return true;
}

bool GTapeNode::doGraph()
{
    int w = Width / 2;
    CXPoint2 conners[CornerCount] =
    {
        CXPoint2 ( mRect.mX - w, mRect.mY - w ),
        CXPoint2 ( mRect.mX - w, mRect.mH / 2 + mRect.mY - w ),
        CXPoint2 ( mRect.mX - w, mRect.bottom() - w ),

        CXPoint2 ( mRect.mX + mRect.mW / 2 - w, mRect.mY - w ),
        CXPoint2 ( mRect.mX + mRect.mW / 2 - w, mRect.bottom() - w ),

        CXPoint2 ( mRect.right() - w, mRect.mY - w ),
        CXPoint2 ( mRect.right() - w, mRect.mH / 2 + mRect.mY - w ),
        CXPoint2 ( mRect.right() - w, mRect.bottom() - w ),
    };
    for ( int i = 0; i < CornerCount; ++i )
    {
        if ( mCorner[i] )
            mCorner[i]->setXY ( conners[i].mX, conners[i].mY );
    }
    return GRectNode::doGraph();
}

void GTapeNode::setColor ( u32 color )
{
    GRectNode::setColor ( color );
    for ( int i = 0; i < CornerCount; ++i )
    {
        if ( mCorner[i] )
            mCorner[i]->setColor ( color );
    }
}
