#include "GGameDemoHeader.h"
#include "GCamera.h"
#include "GTimer.h"
#include "GFrustum.h"

GCamera::GCamera ( void )
{
	mMouseZoomFactor = 720.0F;
	mNear = 0.01f;
	mFieldOfView = ( float ) D3DX_PI / 4.0f;
	mFar = 30000.0f;

	mCanGetInput = true;
	mpTransMan = NULL;

	mbTraceMan = false;

	mfLenTransMan = LEN_TRACE_MAN;

	mCuller = NULL;

	mProj = NORMALMATRIX;

	mView = NORMALMATRIX;

	D9DEVICE->mOnResetDevice += this;
}

GCamera::~GCamera ( void )
{
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
	if ( mCuller == nullptr )
		mCuller = new GFrustum();
	mCuller->update ( this );
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

	D3DXMatrixLookAtLH ( &mView, &getTrans().mTranslate, &vLookAtPos, &getTrans().mUpon );

	if ( D9DEVICE != NULL )
	{
		D9DEVICE->GetDvc()->SetTransform ( D3DTS_VIEW, &mView );
	}
}

void GCamera::getInput ( DWORD frameTimeMs )
{
	if ( !mCanGetInput )
		return;

	if ( INPUTSYSTEM.IsPressKey ( DIK_ADD ) )
		getTrans().mSpeedMove += 0.03f * frameTimeMs;

	if ( INPUTSYSTEM.IsPressKey ( DIK_SUBTRACT ) )
		getTrans().mSpeedMove -= 0.03f * frameTimeMs;

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

	if ( !mpTransMan->getTrans().mCanMoveStep )
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
	//if ( mCuller )
	//    mCuller->Update ( & ( GetWorldMatrix ( false ) ) );
}

void GCamera::onCallBack ( const CXDelegate& delgate )
{
	if ( delgate == D9DEVICE->mOnResetDevice )
	{
		setProj();
	}
}

void GCamera::moveTo ( D3DXMATRIX& matrix )
{
	getTrans().moveTo ( matrix, 3000 );
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

GFrustum* GCamera::getCuller() const
{
	return mCuller;
}

void GCamera::getObjectCorrdPos ( D3DXVECTOR3& out, GNode* obj )
{
	CXASSERT ( obj );
	D3DXMATRIX mat = obj->getTrans().GetWorldMatrix ( false );
	D3DXMatrixInverse ( &mat, 0, &mat );
	out = getTrans().mTranslate;
	D3DXVec3TransformCoord ( &out, &out, &mat );
}





