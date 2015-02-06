#include "GGameDemoHeader.h"
#include "GRectNode.h"
#include "GD9Device.h"
#include "GText.h"

struct GRectVertex
{
    float mX, mY, mZ, mRHW;
    CXColor mColor;
    static const DWORD mFVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE ;
};

GRectNode::GRectNode ( void )
{
    setState ( eUINodeState_CanHover, true );
    setState ( eUINodeState_IsHover, false );
    mVertexColor.Color = Color_Pure_White;
    mRect.mW = 100;
    mRect.mH = 30;

    mGraph.setVB ( mVB );

    mGraph.mType = D3DPT_LINELIST;
    mGraph.mPrimitiveCount = 4;
}


GRectNode::~GRectNode ( void )
{
}

bool GRectNode::recreate()
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
        0, 1, 1, 2, 2, 3, 3, 0
    };

    CXASSERT_RETURN_FALSE ( mIB.fill ( ( void* ) aIndex ) );

    return true;
}



bool GRectNode::render()
{
    if ( !mNodeState[eObjState_Render] )
        return false;
    mIB.set();
    mVB.set();
    if ( mIB.isValid() && mVB.isValid() )
        mGraph.draw();
    return true;
}

void GRectNode::setRect ( long x, long y, long w, long h )
{
    mRect.mX = x;
    mRect.mY = y;
    mRect.mW = w;
    mRect.mH = h;

    doGraph();
}

void GRectNode::setRect ( const CXRect& rect )
{
    mRect = rect;
    doGraph();
}

bool GRectNode::doGraph()
{
    if ( mVB.isValid() )
    {
        static const float fz = 0.1f;
        float rhw = 1.0f;
        GRectVertex aCoord[] =
        {
            { mRect.mX, mRect.bottom(), fz, rhw, mVertexColor},
            { mRect.mX, mRect.mY, fz, rhw, mVertexColor},
            { mRect.right(), mRect.mY, fz, rhw, mVertexColor},
            { mRect.right(), mRect.bottom(), fz, rhw, mVertexColor},
        };
        CXASSERT_RETURN_FALSE ( mVB.fill ( aCoord ) );
    }

    return true;
}

void GRectNode::setColor ( u32 color )
{
    mVertexColor.Color = color;
    doGraph();
}

const CXRect& GRectNode::getRect() const
{
    return mRect;
}

bool GRectNode::containPoint ( long x, long y ) const
{
    return mRect.contain ( x, y );
}


u32 GRectNode::getColor() const
{
    return mVertexColor.Color;
}

