#include "GGameDemoHeader.h"
#include "GUINode.h"
#include "GD9Device.h"
#include "GText.h"
struct GUIVertex
{
    float mX, mY, mZ, mRHW;
    CXColor mColor;
    float mU, mV;
    static const DWORD mFVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
};

GUINode::GUINode ( void )
    : mTextureProp ( nullptr )
{
    setState ( eUINodeState_CanHover, true );
    setState ( eUINodeState_IsHover, false );
    mVertexColor.Color = Color_Pure_White;
    mRect.mW = 100;
    mRect.mH = 30;

    mVB.mElementCount = 4;

    mGraph.mPrimitiveCount = 2;
    mGraph.mType = D3DPT_TRIANGLELIST;
}


GUINode::~GUINode ( void )
{
}

bool GUINode::recreate()
{
    if ( !mVB.isValid() )
    {
        mVB.mElementSize = sizeof ( GUIVertex );
        mVB.mFVF = GUIVertex::mFVF;
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
        0, 1, 2,
        0, 2, 3,
    };

    CXASSERT_RETURN_FALSE ( mIB.fill ( ( void* ) aIndex ) );

    return true;
}



bool GUINode::render()
{
    if ( !__super::render() )
        return false;
    mIB.set();
    mVB.set();
    if ( mTextureProp )
    {
        D9DEVICE->GetDvc()->SetTexture ( 0,  mTextureProp->getTexture() );
        D9DEVICE->renderFirstGraph ( mTextureProp != nullptr );
    }
    D9DEVICE->renderFirstGraph ( mTextureProp != nullptr );
    mGraph.draw();
    return true;
}

void GUINode::setRect ( long x, long y, long w, long h )
{
    mRect.mX = x;
    mRect.mY = y;
    mRect.mW = w;
    mRect.mH = h;

    doGraph();
}

void GUINode::setRect ( const CXRect& rect )
{
    mRect = rect;
    doGraph();
}


bool GUINode::doGraph()
{
    if ( mVB.isValid() )
    {
        static const float fz = 0.1f;
        float rhw = 1.0f;
        GUIVertex aCoord[] =
        {
            { mRect.mX, mRect.bottom(), fz, rhw, mVertexColor, 0, 1},
            { mRect.mX, mRect.mY, fz, rhw, mVertexColor, 0, 0},
            { mRect.right(), mRect.mY, fz, rhw, mVertexColor, 1, 0},
            { mRect.right(), mRect.bottom(), fz, rhw, mVertexColor, 1, 1},
        };
        CXASSERT_RETURN_FALSE ( mVB.fill ( aCoord ) );
    }

    return true;
}

void GUINode::setColor ( u32 color )
{
    mVertexColor.Color = color;
    doGraph();
}

const CXRect& GUINode::getRect() const
{
    return mRect;
}

void GUINode::setTexture ( const char* fileName )
{
    mTextureProp = TextureMgr->getResource ( fileName );
}

bool GUINode::containPoint ( long x, long y ) const
{
    return mRect.contain ( x, y );
}

GUINode* GUINode::getHoverNode ( long x, long y )
{
    GUINode* n = nullptr;
for ( auto child: mChildren )
    {
        GUINode* pc = ( GUINode* ) child;
        n = pc->getHoverNode ( x, y );
        if ( n != nullptr )
            return n;
    }
    if ( mNodeState[eUINodeState_CanHover] && containPoint ( x, y ) )
    {
        return this;
    }
    return nullptr;
}

u32 GUINode::getColor() const
{
    return mVertexColor.Color;
}
