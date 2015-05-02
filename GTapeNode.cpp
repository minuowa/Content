#include "GGameDemoHeader.h"
#include "GTapeNode.h"
#include "Content.h"
#include "GGWireRectNode.h"
#include "GUIManager.h"
#include "GD9Device.h"

static const int Width = 16;

GTapeNode::GTapeNode ( void )
{
    setState ( eUINodeState_AcpectEvent, false );

    dMemoryZeroArray ( mCorner );
    setRenderTech ( GRenderTech_Primite );
    Content::UIMgr.mDelegateNodeButtonDown += this;
    Content::UIMgr.mDelegateHoverNodeChanged += this;

    for ( int i = 0; i < CornerCount; ++i )
    {
        mCorner[i] = new GGWireRectNode();
        mCorner[i]->setRenderTech ( GRenderTech_Primite );
        mCorner[i]->setState ( eObjState_Render, false );
        mCorner[i]->setState ( eUINodeState_CanHover, true );
        mCorner[i]->setState ( eUINodeState_CanCapture, true );
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
    if ( !__super::render() )
        return false;
for ( GRectNode * n : mCorner )
        n->draw();
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
	updateRelRect();
	correctPixel();
	setOrignalRectWithRelRect();
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
    static LPCSTR cursors[CornerCount] =
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

void GTapeNode::drag ( long x, long y )
{
    eCornerType cornertype = CornerCount;
    for ( int i = 0; i < CornerCount; ++i )
    {
        if ( mCorner[i]->mNodeState[eUINodeState_IsCapture] )
            //if ( mCorner[i]->mNodeState[eUINodeState_IsHover] )
        {
            cornertype = eCornerType ( i );
            break;
        }
    }
    if ( cornertype == CornerCount )
    {
        return;
    }
    long x0 = mRect.mX;
    long y0 = mRect.mY;
    long x1 = mRect.right();
    long y1 = mRect.bottom();
    struct DragOperator
    {
        long mOpX0;
        long mOpY0;
        long mOpX1;
        long mOpY1;
    };
    static const DragOperator dragops[CornerCount] =
    {
        {1, 1, 0, 0},
        {1, 0, 0, 0},
        {1, 0, 0, 1},

        {0, 1, 0, 0},
        {0, 0, 0, 1},

        {0, 1, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 1, 1},
    };

    x0 = x * dragops[cornertype].mOpX0 + x0 * ( 1 - dragops[cornertype].mOpX0 );
    y0 = y * dragops[cornertype].mOpY0 + y0 * ( 1 - dragops[cornertype].mOpY0 );
    x1 = x * dragops[cornertype].mOpX1 + x1 * ( 1 - dragops[cornertype].mOpX1 );
    y1 = y * dragops[cornertype].mOpY1 + y1 * ( 1 - dragops[cornertype].mOpY1 );

    mRect.mX = x0;
    mRect.mY = y0;
    mRect.mW = x1 - x0;
    mRect.mH = y1 - y0;
    mRect.correct();
    updateRelRect();
    correctPixel();
    setOrignalRectWithRelRect();
}

long GTapeNode::getRealX() /*const*/
{
    return mRelRect.mX / mScaleX;
}

long GTapeNode::getRealY() /*const*/
{
    return mRelRect.mY / mScaleY;
}

long GTapeNode::getRealW() /*const*/
{
    return mRelRect.mW / mScaleX;
}

long GTapeNode::getRealH() /*const*/
{
    return mRelRect.mH / mScaleY;
}
