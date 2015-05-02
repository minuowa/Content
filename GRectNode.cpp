#include "GGameDemoHeader.h"
#include "GRectNode.h"
#include "GD9Device.h"
#include "GText.h"
#include "Content.h"

GRectNode::GRectNode ( void )
{
    setState ( eObjState_Render, true );
    setState ( eUINodeState_CanHover, false );
    setState ( eUINodeState_IsHover, false );
    setState ( eUINodeState_CanScale, false );

    mVertexColor.Color = Color_Pure_White;
    mRect.mW = 100;
    mRect.mH = 30;

    mGraph.setVB ( mVB );

    mGraph.mRenderTech = GRenderTech_Primite;
    mGraph.mType = D3DPT_TRIANGLELIST;
    mGraph.mPrimitiveCount = 2;

    mScaleX = mScaleY = 1;

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
    mIB.set();
    mVB.set();
    if ( mIB.isValid() && mVB.isValid() )
        mGraph.draw();
    return true;
}

void GRectNode::setRect ( long x, long y, long w, long h )
{
    setRect ( CXRect ( x, y, w, h ) );
}

void GRectNode::setRect ( const CXRect& rect )
{
    mRect = rect;
    updateRelRect();
    correctPixel();
    if ( mNodeState[eUINodeState_CanScale] )
        setOrignalRectWithRelRect();
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
    setRect ( mRect );
}

void GRectNode::setXY ( long x, long y )
{
    mRect.mX = x;
    mRect.mY = y;
    setRect ( mRect );
}

void GRectNode::offset ( long x, long y )
{
    mRect.offset ( x, y );
    updateRelRect();
    correctPixel();
    setOrignalRectWithRelRect();
for ( auto & c: mChildren )
    {
        auto child = ( GRectNode* ) c;
        child->offset ( x, y );
    }
}

void GRectNode::setRelRect ( const CXRect& rect )
{
    mRelRect = rect;
    correctPixel();
    updateAbsoluteRect();
}

void GRectNode::updateAbsoluteRect()
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
    mDelegateOnRectChanged.trigger();
    doGraph();

    //if ( mNodeState[eUINodeState_CanScale] )
    //    setOrignalRectWithAbsoluteRect();

for ( auto child: mChildren )
    {
        GRectNode* n = ( GRectNode* ) child;
        n->updateAbsoluteRect();
    }
}
void GRectNode::onPropertyChangeEnd ( void* cur )
{
    if ( cur == &mRelRect.mX
            || cur == &mRelRect.mY
            || cur == &mRelRect.mW
            || cur == &mRelRect.mH
       )
    {
        updateAbsoluteRect();
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

void GRectNode::setRB ( long r, long b )
{
    mRect.mW = r - mRect.mX;
    mRect.mH = b - mRect.mY;
    updateRelRect();
    correctPixel();
    setOrignalRectWithRelRect();
}

void GRectNode::updateRelRect()
{
    if ( mParent != nullptr )
    {
        GRectNode* np = ( GRectNode* ) mParent;
        mRelRect.mX = mRect.mX - np->mRect.mX;
        mRelRect.mY = mRect.mY - np->mRect.mY;
        mRelRect.mW = mRect.mW;
        mRelRect.mH = mRect.mH;
    }
    else
    {
        mRelRect = mRect;
    }
    mDelegateOnRectChanged.trigger();
    doGraph();
}

void GRectNode::correct()
{
    mRect.correct();
    mRelRect.correct();
    correctPixel();
}

void GRectNode::clamp ( const CXRect& rc )
{
    mRect.clamp ( rc );
    setRect ( mRect );
}

void GRectNode::scale ( double dscale, long xref, long yref )
{
    CXASSERT ( mNodeState.get ( eUINodeState_CanScale ) );
    mRect.scale ( dscale / mScaleX, xref, yref );
    updateRelRect();
    correctPixel();

    mScaleX = dscale;
    mScaleY = dscale;

for ( auto & child: mChildren )
    {
        auto c = ( GRectNode* ) child;
        if ( c->mNodeState[eUINodeState_CanScale] )
            c->scale ( dscale );
    }
}

void GRectNode::scale ( double dscale )
{
    mScaleX = dscale;
    mScaleY = dscale;
    long x = mOrignalRelRect.mX * dscale;
    long y = mOrignalRelRect.mY * dscale;
    long w = mOrignalRelRect.mW * dscale;
    long h = mOrignalRelRect.mH * dscale;
    mRelRect.mX = x;
    mRelRect.mY = y;
    mRelRect.mW = w;
    mRelRect.mH = h;
    correctPixel();
    updateAbsoluteRect();
for ( auto & child: mChildren )
    {
        auto c = ( GRectNode* ) child;
        if ( c->mNodeState[eUINodeState_CanScale] )
            c->scale ( dscale );
    }
}

double GRectNode::getScaleX() const
{
    return mScaleX;
}

void GRectNode::setOrignalRelRect ( const CXRect& relRect )
{
    int r = relRect.right();
    int b = relRect.bottom();
    int x = dRound ( relRect.mX, int ( mScaleX ) ) ;
    int y = dRound ( relRect.mY, int ( mScaleX ) ) ;
    int w = dRound ( ( r - x ), int ( mScaleX ) ) / int ( mScaleX );
    int h = dRound ( ( b - y ), int ( mScaleX ) ) / int ( mScaleX );

    mOrignalRelRect.mW = w;
    mOrignalRelRect.mH = h;

    x = dRound ( relRect.mX, int ( mScaleX ) ) / int ( mScaleX );
    y = dRound ( relRect.mY, int ( mScaleX ) ) / int ( mScaleX );

    mOrignalRelRect.mX = x;
    mOrignalRelRect.mY = y;
}

double GRectNode::getScaleY() const
{
    return mScaleY;
}

void GRectNode::setOrignalRectWithRelRect()
{
    setOrignalRelRect ( mRelRect );
for ( auto & child: mChildren )
    {
        auto c = ( GRectNode* ) child;
        c->setOrignalRectWithRelRect();
    }
}

GNode* GRectNode::addChild ( GRectNode* c )
{
    __super::addChild ( ( GNode* ) c );
    if ( c )
    {
        c->updateRelRect();
        c->correctPixel();
    }
    return c;
}

void GRectNode::updateRect()
{

}

void GRectNode::setRenderTech ( GRenderTech tech )
{
    mGraph.mRenderTech = tech;
}

void GRectNode::correctPixel()
{
    if ( !mNodeState[eUINodeState_CorrectPixel] )
        return;
    int r = mRelRect.right();
    int b = mRelRect.bottom();
    int x = dRound ( mRelRect.mX, int ( mScaleX ) );
    int y = dRound ( mRelRect.mY, int ( mScaleX ) );
    int w = dRound ( ( r - x ), int ( mScaleX ) );
    int h = dRound ( ( b - y ), int ( mScaleX ) );
    mRelRect.mX = x;
    mRelRect.mY = y;
    mRelRect.mW = w;
    mRelRect.mH = h;
    updateAbsoluteRect();
}


