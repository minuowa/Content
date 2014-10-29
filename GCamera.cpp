#include "GGameDemoHeader.h"
#include "GCamera.h"
//#include "GXNode.h"
#include "GTimer.h"
//CEye gEye;

GCamera::GCamera ( void )
{
    mCanGetInput = true;
    mpTransMan = NULL;

    mbTraceMan = false;

    mfLenTransMan = LEN_TRACE_MAN;

    mpEyeCliper = NULL;

    matProj = NORMALMATRIX;

    matView = NORMALMATRIX;

    D9DEVICE->mOnResetDevice += this;
}

GCamera::~GCamera ( void )
{
    dSafeDelete ( mpEyeCliper );
	D9DEVICE->mOnResetDevice -= this;
}

void GCamera::setProj()
{
    float aspect = ( ( float ) ( D9DEVICE->mWidth ) ) / ( ( float ) ( D9DEVICE->mHeight ) );

    D3DXMatrixPerspectiveFovLH ( &matProj, D3DX_PI / 4.0f, aspect, 1.0f, Max_Eye_Distance );

    if ( D9DEVICE != NULL )
    {
        D9DEVICE->GetDvc()->SetTransform ( D3DTS_PROJECTION, &matProj );
    }
    if ( mpEyeCliper == nullptr )
        mpEyeCliper = new GCuller ( Max_Eye_Distance, D3DX_PI / 4.0f, aspect );

}


bool GCamera::recreate()
{
    if ( !__super::recreate() )
        return false;

    return true;
}

void GCamera::setView()
{
    D3DXVECTOR3 vLookAtPos = getTrans().mTranslate + getTrans().mDir * 100;

    D3DXMatrixLookAtLH ( &matView, &getTrans().mTranslate, &vLookAtPos, &getTrans().mUpon );

    if ( D9DEVICE != NULL )
    {
        D9DEVICE->GetDvc()->SetTransform ( D3DTS_VIEW, &matView );
    }
}

void GCamera::getInput ( DWORD frameTimeMs )
{
	if (!mCanGetInput)
		return;

    if ( INPUTSYSTEM.IsPressKey ( DIK_ADD ) )
        getTrans().mfSpeedMove += 0.03f * frameTimeMs;

    if ( INPUTSYSTEM.IsPressKey ( DIK_SUBTRACT ) )
        getTrans().mfSpeedMove -= 0.03f * frameTimeMs;

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

    getTrans().MoveStep ( vMove.z / 120 * 5.0f );

    if ( INPUTSYSTEM.isPressingButton ( eButtonType_RightButton ) )
    {
        getTrans().TrunStepLeftRightWithUp ( -vMove.x / 800.0f );

        getTrans().TrunWithRight ( -vMove.y / 800.0f );
    }
}


void GCamera::TraceMan(  )
{
    if ( mpTransMan == NULL || !mbTraceMan )
    {
        return;
    }

    if ( !mpTransMan->getTrans().mbCanMoveStep )
    {
        return;
    }

    D3DXVECTOR3 Pos = ZEROVECTOR3;

    Pos = mpTransMan->getTrans().mTranslate - mpTransMan->getTrans().mDir * mfLenTransMan * 2;

    Pos += mpTransMan->getTrans().mUpon * mfLenTransMan;

    D3DXVECTOR3 vDist = Pos - getTrans().mTranslate;


    D3DXVECTOR3 Dir = mpTransMan->getTrans().mTranslate - Pos;

    getTrans().mTranslate = Pos;

    getTrans().mDir = Dir;

    D3DXVec3Cross ( &getTrans().mRight, & ( D3DXVECTOR3 ( 0, 1, 0 ) ), &getTrans().mDir );

    D3DXVec3Cross ( &getTrans().mUpon, &getTrans().mDir, &getTrans().mRight );

    D3DXVec3Normalize ( &getTrans().mDir, &getTrans().mDir );
    D3DXVec3Normalize ( &getTrans().mUpon, &getTrans().mUpon );
    D3DXVec3Normalize ( &getTrans().mRight, &getTrans().mRight );

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
    if ( mpEyeCliper )
        mpEyeCliper->Update ( & ( GetWorldMatrix ( false ) ) );
}

void GCamera::onCallBack ( const CXDelegate& delgate )
{
    if ( delgate == D9DEVICE->mOnResetDevice )
    {
        setProj();
    }
}



