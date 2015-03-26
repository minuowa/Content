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

    mGraph.mType = D3DPT_TRIANGLELIST;
    mGraph.mPrimitiveCount = 2;
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
        mIB.mIndexCount = 6;
        CXASSERT_RETURN_FALSE ( mIB.recreate ( D3DPOOL_MANAGED ) );
    }
    DWORD aIndex[6] =
    {
        0, 1, 2, 0, 2, 3,
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
    mRelRect.mW = w;
    mRelRect.mH = h;
    doGraph();
}

void GRectNode::setRect ( const CXRect& rect )
{
    mRect = rect;
    mRelRect.mW = rect.mW;
    mRelRect.mH = rect.mH;
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

void GRectNode::setWH ( long w, long h )
{
    mRect.mW = w;
    mRect.mH = h;
    doGraph();
}

void GRectNode::setXY ( long x, long y )
{
    mRect.mX = x;
    mRect.mY = y;
    doGraph();
}

void GRectNode::offset ( long x, long y )
{
    if ( mParent != nullptr )
	{
		mRelRect.offset ( x, y );
		updateAbRect();
	}
    else
	{
		mRect.offset ( x, y );
		doGraph();
	}
}

void GRectNode::setRelRect ( const CXRect& rect )
{
    mRelRect = rect;
    updateAbRect();
}

void GRectNode::updateAbRect()
{
    if ( mParent != nullptr )
    {
        GRectNode* np = ( GRectNode* ) mParent;
        mRect.mX = mRelRect.mX + np->mRect.mX;
        mRect.mY = mRelRect.mY + np->mRect.mY;
        mRect.mW = mRelRect.mW;
        mRect.mH = mRelRect.mH;
    }
    else
    {
        mRect = mRelRect;
    }
    doGraph();

for ( auto child: mChildren )
    {
        GRectNode* n = ( GRectNode* ) child;
        n->updateAbRect();
    }
}
void GRectNode::onPropertyChangeEnd( void* cur )
{
	if (cur==&mRelRect.mX
		||cur==&mRelRect.mY
		||cur==&mRelRect.mW
		||cur==&mRelRect.mH
		)
	{
		updateAbRect();
	}
}
void GRectNode::registerAllProperty()
{
	__super::registerAllProperty();
	__RegisterProperty ( mRelRect.mX );
	__RegisterProperty ( mRelRect.mY );
	__RegisterProperty ( mRelRect.mW );
	__RegisterProperty ( mRelRect.mH );
}

