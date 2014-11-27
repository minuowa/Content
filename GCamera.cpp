#include "GGameDemoHeader.h"
#include "GCamera.h"
#include "GTimer.h"
#include "GFrustum.h"
#include "GTerrainCommon.h"

GCamera::GCamera ( void )
{
    mMouseZoomFactor = 720.0F;
    mNear = 0.01f;
    mFieldOfView = ( float ) D3DX_PI / 4.0f;
    mFar = 100000.0f;

    setCanGetInput ( true );

    mpTransMan = NULL;

    mbTraceMan = false;

    mfLenTransMan = LEN_TRACE_MAN;

    mFrustum = NULL;
    mCuller = nullptr;

    mProj = NORMALMATRIX;

    mView = NORMALMATRIX;

    D9DEVICE->mOnResetDevice += this;
}

GCamera::~GCamera ( void )
{
    dSafeDelete ( mFrustum );
    dSafeDelete ( mCuller );
    D9DEVICE->mOnResetDevice -= this;
}

void GCamera::setProj()
{
    mAspect = ( ( float ) ( D9DEVICE->mWidth ) ) / ( ( float ) ( D9DEVICE->mHeight ) );

    D3DXMatrixPerspectiveFovLH ( &mProj, mFieldOfView, mAspect, mNear, mFar );

    if ( D9DEVICE != NULL )
    {
        D9DEVICE->GetDvc()->SetTransform ( D3DTS_PROJECTION, &mProj );
    }
    if ( mFrustum == nullptr )
    {
        mFrustum = new GFrustum();
        mFrustum->init ( this );
        mCuller = new GCubeBound;
        mCuller->mCenter = mFrustum->mCenter;
        mCuller->mRadius = mFrustum->mRadius;
    }
}


bool GCamera::recreate()
{
    if ( !__super::recreate() )
        return false;

    return true;
}

void GCamera::setView()
{
    D3DXVECTOR3 vLookAtPos = getTrans()->getWorldTranslate() + getTrans()->getWorldDir() * 100;

    D3DXMatrixLookAtLH ( &mView, &getTrans()->getWorldTranslate(), &vLookAtPos, &getTrans()->getWorldUpon() );

    if ( D9DEVICE != NULL )
    {
        D9DEVICE->GetDvc()->SetTransform ( D3DTS_VIEW, &mView );
    }
}

void GCamera::getInput ( DWORD frameTimeMs )
{
    if ( INPUTSYSTEM.IsPressKey ( DIK_ADD ) )
        getTrans()->mSpeedMove += 0.03f * frameTimeMs;

    if ( INPUTSYSTEM.IsPressKey ( DIK_SUBTRACT ) )
        getTrans()->mSpeedMove -= 0.03f * frameTimeMs;

    POINT pt = INPUTSYSTEM.GetMousePoint();

    if ( pt.x < 0 || pt.x > D9DEVICE->mWidth || pt.y < 0 || pt.y > D9DEVICE->mHeight )
    {
        return;			//鼠标在客户区外面就不执行GetInput
    }
    D3DXVECTOR3 vMove = INPUTSYSTEM.GetMouseMoveEX();

    if ( INPUTSYSTEM.IsPressKey ( DIK_A ) || INPUTSYSTEM.IsPressKey ( DIK_D ) || INPUTSYSTEM.IsPressKey ( DIK_S ) || INPUTSYSTEM.IsPressKey ( DIK_W ) )
    {
        mbTraceMan = true;
    }
    else
    {
        mbTraceMan = false;
    }

    TraceMan();

    if ( mbTraceMan )
    {
        float fDelta = vMove.z / 120 * 10.0f;

        if ( mfLenTransMan - fDelta > 5.0f )
        {
            mfLenTransMan -= fDelta;
        }

    }

    getTrans()->MoveStep ( vMove.z / 120 * 5.0f );

    if ( INPUTSYSTEM.isPressingButton ( eButtonType_RightButton ) )
    {
        getTrans()->trunWithUpon ( -vMove.x / 800.0f );

        getTrans()->trunWithRight ( -vMove.y / 800.0f );
    }
    updateWorld();
}


void GCamera::TraceMan(  )
{
//   if ( mpTransMan == NULL || !mbTraceMan )
//   {
//       return;
//   }

//   if ( !mpTransMan->getTrans()->mCanMoveStep )
//   {
//       return;
//   }

//   D3DXVECTOR3 Pos = ZEROVECTOR3;

//   Pos = mpTransMan->getTrans()->mMatLocal.mTranslate - mpTransMan->getTrans()->getDir() * mfLenTransMan * 2;

//   Pos += mpTransMan->getTrans()->mMatLocal.mUpon * mfLenTransMan;

//   D3DXVECTOR3 vDist = Pos - getTrans()->mMatLocal.mTranslate;


//   D3DXVECTOR3 Dir = mpTransMan->getTrans()->mMatLocal.mTranslate - Pos;

//   getTrans()->mMatLocal.mTranslate = Pos;

//   getTrans()->setDir(Dir) ;

//   D3DXVec3Cross ( &getTrans()->mMatLocal.mRight, & ( D3DXVECTOR3 ( 0, 1, 0 ) ), &getTrans()->getDir() );

//   D3DXVec3Cross ( &getTrans()->mMatLocal.mUpon, &getTrans()->getDir(), &getTrans()->mMatLocal.mRight );
    //getTrans()->normalizeRotation();

}


bool GCamera::InitTrack ( GNode *pWorldObj )
{
    if ( pWorldObj != NULL )
    {
        mpTransMan = pWorldObj;

        return true;
    }
    else
    {
        return false;
    }
}

void GCamera::update()
{
    __super::update();
	getTrans()->update();
}

void GCamera::onCallBack ( const CXDelegate& delgate, CXEventArgs* )
{
    if ( delgate == D9DEVICE->mOnResetDevice )
    {
        setProj();
    }
}

void GCamera::moveTo( const GMatrix& matrix,u32 timeMS )
{
    getTrans()->moveTo ( matrix, timeMS );
}

void GCamera::cliper ( GNode* n )
{

}

float GCamera::getNear() const
{
    return mNear;
}

float GCamera::getFar() const
{
    return mFar;
}

float GCamera::getFov() const
{
    return mFieldOfView;
}

float GCamera::getAspect() const
{
    return mAspect;
}

GFrustum* GCamera::getFrustum() const
{
    return mFrustum;
}

void GCamera::getObjectCorrdPos ( D3DXVECTOR3& out, GNode* obj )
{
    CXASSERT ( obj );
    D3DXMATRIX mat = obj->getTrans()->getLocalD3D();
    //obj->getTrans()->GetWorldMatrix ( mat, false );
    out = getTrans()->getTranslate();
    D3DXMatrixInverse ( &mat, 0, &mat );
    dMatrixTranslateVec3 ( out, mat );
}

void GCamera::updateCullerToObjectCoord ( GNode* obj )
{
    D3DXVECTOR3 center = mFrustum->mCenter;
    D3DXMATRIX matWorld = getTrans()->getLocalD3D();
    //getTrans()->GetWorldMatrix ( matWorld );
    dMatrixTranslateVec3 ( center, matWorld );

    D3DXMATRIX objWorld = obj ->getTrans()->getLocalD3D();
    dMatrixInverse ( objWorld );

    dMatrixTranslateVec3 ( center, objWorld );

    mCuller->mCenter = center;
}

bool GCamera::isInCamera ( GCubeBound* bound ) const
{
    return dVector3Length ( mCuller->mCenter - bound->mCenter ) < ( mCuller->mRadius + bound->mRadius );
}





