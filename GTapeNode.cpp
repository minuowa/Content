#include "GGameDemoHeader.h"
#include "GTapeNode.h"
#include "Content.h"
#include "GGWireRectNode.h"

static const int Width = 20;

GTapeNode::GTapeNode ( void )
{
    setState ( eUINodeState_AcpectEvent, false );

    dMemoryZeroArray ( mCorner );

    Content::UIMgr.mDelegateNodeButtonDown += this;
    Content::UIMgr.mDelegateHoverNodeChanged += this;

    for ( int i = 0; i < CornerCount; ++i )
    {
        mCorner[i] = new GGWireRectNode();
        mCorner[i]->setState ( eObjState_Render, false );
        mCorner[i]->setState ( eUINodeState_CanHover, true );
        mCorner[i]->setState ( eUINodeState_AcpectEvent, true );
        mCorner[i]->recreate();
        mCorner[i]->setWH ( Width, Width );
        addChild ( mCorner[i] );
    }
}


GTapeNode::~GTapeNode ( void )
{
    Content::UIMgr.mDelegateNodeButtonDown -= this;
    Content::UIMgr.mDelegateHoverNodeChanged -= this;
}

bool GTapeNode::recreate()
{
    return GGWireRectNode::recreate();
}

bool GTapeNode::render()
{
    Content::Device.renderFirstGraph ( false );
    if ( !__super::render() )
        return false;
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
            mCorner[i]->setRect ( conners[i].mX, conners[i].mY, Width, Width );
    }
    return __super::doGraph();
}

void GTapeNode::setColor ( u32 color )
{
    GGWireRectNode::setColor ( color );
    for ( int i = 0; i < CornerCount; ++i )
    {
        if ( mCorner[i] )
            mCorner[i]->setColor ( color );
    }
}

void GTapeNode::scale ( float scale, long xref, long yref )
{
    __super::scale ( scale, xref, yref );
//for ( auto & c: mCorner )
//    {
//        c->scale ( scale, xref, yref );
//    }
}

void GTapeNode::offset ( long x, long y )
{
    mRect.offset ( x, y );
    if ( mNodeState[eUINodeState_CanScale] )
        mOrignalRect.offset ( x, y );
    updateRelRect();
}

void GTapeNode::onCallBack ( const CXDelegate& d, CXEventArgs* e )
{
    if ( d == Content::UIMgr.mDelegateHoverNodeChanged )
    {
        GUIHoverNodeChangedEvent* arg = ( GUIHoverNodeChangedEvent* ) e;

        for ( int i = 0; i < CornerCount; ++i )
        {
            if ( mCorner[i] == arg->mOldNode )
            {
                HCURSOR hCur;
                hCur = LoadCursor ( NULL, IDC_ARROW );
                SetCursor ( hCur );
            }
        }

        for ( int i = 0; i < CornerCount; ++i )
        {
            if ( mCorner[i] == arg->mNewNode )
            {
                changeCursor ( eCornerType ( i ) );
            }
        }
    }
}

void GTapeNode::changeCursor ( eCornerType type )
{
    HCURSOR hCur;
    static LPCWSTR cursors[CornerCount] =
    {
        IDC_SIZENWSE,
        IDC_SIZEWE,
        IDC_SIZENESW,

        IDC_SIZENS,
        IDC_SIZENS,

        IDC_SIZENESW,
        IDC_SIZEWE,
        IDC_SIZENWSE,
    };
    hCur = LoadCursor ( NULL, cursors[type] );
    SetCursor ( hCur );
}
