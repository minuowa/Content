#include "GGameDemoHeader.h"
#include "GRectCursor.h"

static const u32 LineCount = 4096;
static const int gMinScale = 5;
GRectCursor::GRectCursor ( void )
    : mTarget ( 0 )
{
    mGraph.mRenderTech = GRenderTech_Primite;
    mGraph.mType = D3DPT_LINELIST;
    mVertexColor.Color = 0X88666666;
    setState ( eObjState_Render, false );
    setState ( eUINodeState_CanScale, true );
}


GRectCursor::~GRectCursor ( void )
{
    if ( mTarget )
    {
        mTarget->mDelegateOnRectChanged -= this;
    }
}

void GRectCursor::offset ( long x, long y )
{
    //mCenter.mX += x;
    //mCenter.mY += y;
    //doGraph();
}

bool GRectCursor::doGraph()
{
    if ( mVB.isValid() )
    {
        static const float fz = 0.1f;
        float rhw = 1.0f;
        int width = dRound ( mRect.mW , int ( mScaleX  ) )/int(mScaleX) + 1;
        int height = dRound ( mRect.mH , int ( mScaleX  ) )/int(mScaleX) + 1;
        GRectVertex* aCorrd = new GRectVertex[ ( width + height ) * 2];
        ///coloum
        for ( int i = 0; i < width; ++i )
        {
            aCorrd[i * 2].mX = mRect.mX + i * mScaleX;
            aCorrd[i * 2].mY = mRect.mY;
            aCorrd[i * 2].mZ = fz;
            aCorrd[i * 2].mRHW = rhw;
            aCorrd[i * 2].mColor = mVertexColor;

            aCorrd[i * 2 + 1].mX = mRect.mX + i * mScaleX;
            aCorrd[i * 2 + 1].mY = mRect.bottom();
            aCorrd[i * 2 + 1].mZ = fz;
            aCorrd[i * 2 + 1].mRHW = rhw;
            aCorrd[i * 2 + 1].mColor = mVertexColor;
        }
        int base = width * 2;
        for ( int i = 0; i < height; ++i )
        {
            aCorrd[i * 2 + base].mX = mRect.mX;
            aCorrd[i * 2 + base].mY = mRect.mY + i * mScaleX;
            aCorrd[i * 2 + base].mZ = fz;
            aCorrd[i * 2 + base].mRHW = rhw;
            aCorrd[i * 2 + width * 2].mColor = mVertexColor;

            aCorrd[i * 2 + base + 1].mX = mRect.right();
            aCorrd[i * 2 + base + 1].mY = mRect.mY + i * mScaleX;
            aCorrd[i * 2 + base + 1].mZ = fz;
            aCorrd[i * 2 + base + 1].mRHW = rhw;
            aCorrd[i * 2 + base + 1].mColor = mVertexColor;
        }
        mGraph.mPrimitiveCount = width + height;
        CXASSERT_RETURN_FALSE ( mVB.fill ( aCorrd , ( width + height ) * 2 ) );
        delete []aCorrd;
    }
    return true;
}

bool GRectCursor::recreate()
{
    if ( !mVB.isValid() )
    {
        mVB.mElementSize = sizeof ( GRectVertex );
        mVB.mFVF = GRectVertex::mFVF;
        mVB.mElementCount = LineCount * 4;
        CXASSERT_RETURN_FALSE ( mVB.recreate ( D3DPOOL_MANAGED ) );
    }

    CXASSERT_RETURN_FALSE ( doGraph() );

    if ( !mIB.isValid() )
    {
        mIB.mIndexCount = LineCount * 4;
        CXASSERT_RETURN_FALSE ( mIB.recreate ( D3DPOOL_MANAGED ) );
    }
    DWORD aIndex[LineCount * 4];

    for ( u32 i = 0; i < LineCount * 4; ++i )
        aIndex[i] = i;

    CXASSERT_RETURN_FALSE ( mIB.fill ( ( void* ) aIndex ) );

    return true;
}

void GRectCursor::scale( double dscale )
{
	__super::scale ( dscale );
	setState ( eObjState_Render, mScaleX >= gMinScale );
}

void GRectCursor::updateByRect ( const CXRect& rc )
{
    if ( mScaleX < gMinScale )
        return;
    setState ( eObjState_Render, mScaleX >= gMinScale );
    mRect = rc;
    doGraph();
}

void GRectCursor::setTarget ( GRectNode* node )
{
    CXASSERT ( node != nullptr );
    mTarget = node;
    node->mDelegateOnRectChanged += this;
    node->mDelegateOnDestory += this;
}

void GRectCursor::onCallBack ( const CXDelegate& d, CXEventArgs* e )
{
    if ( mTarget )
    {
        if ( d == mTarget->mDelegateOnRectChanged )
        {
            updateByRect ( mTarget->getRect() );
        }
        else if ( d == mTarget->mDelegateOnDestory )
        {
            mTarget = nullptr;
        }
    }
}

void GRectCursor::updateAbsoluteRect()
{
}

void GRectCursor::updateRelRect()
{
}
