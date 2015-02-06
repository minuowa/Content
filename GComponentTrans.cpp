#include "GGameDemoHeader.h"
#include "GComponentTrans.h"
#include "GTimer.h"
#include "GD9Device.h"
#include "GNode.h"

static const float DEAULT_SPEED_TRUN = 2.0f;
static const float DEFAULT_SPEED_MOVE = 1.0f;

GComponentTrans::GComponentTrans ( void )
{
    mAutoMoveInfo = nullptr;
    mAutoMove = false;
    mZoom = D3DXVECTOR3 ( 1.0f, 1.0f, 1.0f );
    mDegreeYaw = 0;
    mDegreeRoll = 0;
    mDegreePitch = 0;
    mvLastPos = ZEROVECTOR3;
    //mWorld.mTranslate = ZEROVECTOR3;
    //mWorld.mRight = D3DXVECTOR3 ( 1.0f, 0.0f, 0.0f );
    //mWorld.mUpon = D3DXVECTOR3 ( 0.0f, 1.0f, 0.0f );
    //mWorld.mDir = D3DXVECTOR3 ( 0.0f, 0.0f, 1.0f );

    mSpeedTrun = DEAULT_SPEED_TRUN;
    mSpeedMove = DEFAULT_SPEED_MOVE;
    noitifyWorldNeedUpdate();
    mSpeed = ZEROVECTOR3;

    mBodyPass = ZEROVECTOR3;
    mBodyRote = ZEROVECTOR3;

    mTargetPos = ZEROVECTOR3;

    mTargetDir = ZEROVECTOR3;

    mAutoMove = false;

    mAutoTrun = false;

    mJump = false;

    mCanMoveStep = true;

    mBack = false;
}

GComponentTrans::~GComponentTrans ( void )
{
    dSafeDelete ( mAutoMoveInfo );
}

//void GComponentTrans::GetWorldMatrix ( D3DXMATRIX& out, bool bForTrans /*= false */ )
//{
//    D3DXMATRIX matWorld;    //ÊÀ½ç¾ØÕó
//
//    D3DXMATRIX matScale;	    //Ëõ·Å¾ØÕó
//
//    D3DXMATRIX matTranslation;	//Æ½ÒÆ¾ØÕó
//
//    D3DXMATRIX matRotation;  //Ðý×ª¾ØÕó
//
//    D3DXMatrixIdentity ( &out );
//
//    if ( !bForTrans )
//    {
//        //¼ÆËãËõ·Å¾ØÕó
//        D3DXMatrixScaling ( &matScale, mZoom.x, mZoom.y, mZoom.z );
//        out *= matScale;
//
//        //¼ÆËãÐý×ª¾ØÕó
//        D3DXMatrixRotationYawPitchRoll ( &matRotation, mBodyRote.y, mBodyRote.x, mBodyRote.z );
//        out *= matRotation;
//
//    }
//
//	normalizeRotation();
//
//    out *= D3DXMATRIX (
//               mMatLocal.mRight.x, mMatLocal.mRight.y, mMatLocal.mRight.z, 0,
//               mMatLocal.mUpon.x, mMatLocal.mUpon.y, mMatLocal.mUpon.z, 0,
//               mMatLocal.mDir.x, mMatLocal.mDir.y, mMatLocal.mDir.z, 0,
//               0.0f, 0.0f, 0.0f, 1.0f
//           );
//
//    //¼ÆËãÆ½ÒÆ¾ØÕó
//    if ( bForTrans )
//    {
//        D3DXMatrixTranslation ( &matTranslation, mMatLocal.mTranslate.x, mMatLocal.mTranslate.y, mMatLocal.mTranslate.z );
//    }
//    else
//    {
//        D3DXMatrixTranslation ( &matTranslation, mMatLocal.mTranslate.x + mBodyPass.x * mZoom.x, mMatLocal.mTranslate.y + mBodyPass.y * mZoom.y, mMatLocal.mTranslate.z + mBodyPass.z * mZoom.z );
//    }
//
//    out *= matTranslation;
//}
//

D3DXVECTOR3 GComponentTrans::MoveStep ( float fPass )
{

    if ( !mCanMoveStep )
    {
        return ZEROVECTOR3;
    }

    float fLen = mSpeedMove * fPass;

    normalizeDir();

    mvLastPos = mMatLocal.mTranslate;

    mMatLocal.mTranslate += fLen * mMatLocal.mDir;

    return mMatLocal.mTranslate;
}

void GComponentTrans::TrunStepLeftRight ( float fPara )
{
    D3DXMATRIX matRotation;
    D3DXVECTOR3 vRotation ( mMatLocal.mUpon );
    D3DXMatrixRotationAxis ( &matRotation, &vRotation, fPara );

    D3DXVec3TransformCoord ( &mMatLocal.mRight, &mMatLocal.mRight, &matRotation );
    D3DXVec3TransformCoord ( &mMatLocal.mDir, &mMatLocal.mDir, &matRotation );
    normalizeRotation();
    noitifyWorldNeedUpdate();
    mNeedUpdateYawPitchRow = true;
}

void GComponentTrans::trunWithRight ( float fPara )
{
    D3DXMATRIX matRotation;
    D3DXVECTOR3 vRotation ( mMatLocal.mRight );
    D3DXMatrixRotationAxis ( &matRotation, &vRotation, fPara );

    D3DXVec3TransformCoord ( &mMatLocal.mUpon, &mMatLocal.mUpon, &matRotation );
    D3DXVec3TransformCoord ( &mMatLocal.mDir, &mMatLocal.mDir, &matRotation );
    D3DXVec3Cross ( &mMatLocal.mRight, &mMatLocal.mUpon, &mMatLocal.mDir );

    normalizeRotation();
    noitifyWorldNeedUpdate();
    mNeedUpdateYawPitchRow = true;
}


D3DXVECTOR3 GComponentTrans::SetDirWithUpon ( D3DXVECTOR3 vUpon )
{

    D3DXVec3Cross ( &mMatLocal.mRight, &vUpon, &mMatLocal.mDir );

    D3DXVec3Cross ( &mMatLocal.mDir, &mMatLocal.mRight, &vUpon );

    normalizeRotation();
    mNeedUpdateYawPitchRow = true;

    return mMatLocal.mDir;
}

void GComponentTrans::trunWithUpon ( float fpara )
{
    D3DXMATRIX matRotation;
    D3DXVECTOR3 vRotation ( ZEROFLOAT, 1.0f, ZEROFLOAT );
    D3DXMatrixRotationAxis ( &matRotation, &vRotation, fpara );

    D3DXVec3TransformCoord ( &mMatLocal.mUpon, &mMatLocal.mUpon, &matRotation );
    D3DXVec3TransformCoord ( &mMatLocal.mDir, &mMatLocal.mDir, &matRotation );
    D3DXVec3Cross ( &mMatLocal.mRight, &mMatLocal.mUpon, &mMatLocal.mDir );

    normalizeRotation();
}

int GComponentTrans::Jump()
{
    if ( !mJump )
    {
        mAutoMove = true;

        mCanMoveStep = false;

        mSpeed.y = JUMP_HEIGHT / 0.5f / JUMP_TIME;

        dToggle ( mJump );
    }

    return TRUE_INT;

}

D3DXVECTOR3 GComponentTrans::MoveToPos ( D3DXVECTOR3 vTarget )
{
    mAutoMove = true;

    mTargetPos = vTarget;

    return mTargetPos;
}


//D3DXMATRIX GComponentTrans::GetRotationMatrix ( bool bForTrans/*=false*/ )
//{
//    D3DXMATRIX matRotation;
//    D3DXMatrixIdentity ( &matRotation );
//
//    if ( !bForTrans )
//    {
//        D3DXMatrixRotationYawPitchRoll ( &matRotation, mBodyRote.y, mBodyRote.x, mBodyRote.z );
//    }
//    matRotation *= D3DXMATRIX (
//                       mWorld.mRight.x, mWorld.mRight.y, mWorld.mRight.z, 0,
//                       mWorld.mUpon.x, mWorld.mUpon.y, mWorld.mUpon.z, 0,
//                       mWorld.mDir.x, mWorld.mDir.y, mWorld.mDir.z, 0,
//                       0.0f, 0.0f, 0.0f, 1.0f
//                   );
//    return matRotation;
//}



//D3DXMATRIX GComponentTrans::GetScaleMatrix ( bool bForTrans/*=false*/ )
//{
//    D3DXMATRIX matScaling = NORMALMATRIX;
//
//    if ( !bForTrans )
//    {
//        D3DXMatrixScaling ( &matScaling, mZoom.x, mZoom.y, mZoom.z );
//    }
//
//    return matScaling;
//}

//D3DXMATRIX GComponentTrans::GetTransLation ( bool bForTrans/*=false*/ )
//{
//    D3DXMATRIX matTranslation = NORMALMATRIX;
//
//    if ( bForTrans )
//    {
//        D3DXMatrixTranslation ( &matTranslation, mWorld.mTranslate.x, mWorld.mTranslate.y, mWorld.mTranslate.z );
//    }
//    else
//    {
//        D3DXMatrixTranslation ( &matTranslation, mWorld.mTranslate.x + mBodyPass.x * mZoom.x, mWorld.mTranslate.y + mBodyPass.y * mZoom.y, mWorld.mTranslate.z + mBodyPass.z * mZoom.z );
//    }
//
//    return matTranslation;
//}


D3DXVECTOR3 GComponentTrans::TrunToDir ( D3DXVECTOR3 vTargetDir )
{
    mAutoTrun = true;

    D3DXVec3Normalize ( &mTargetDir, &vTargetDir );

    return mTargetDir;
}

void GComponentTrans::update(  )
{
    if ( mNeedUpdateYawPitchRow )
    {
        updateDirUponRight2YawPitchRoll();
        mNeedUpdateYawPitchRow = false;
    }
    if ( mNeedUpdate )
    {
        if ( mOwner )
            mOwner->updateWorld();
        mNeedUpdate = false;
    }
    if ( mAutoMoveInfo )
    {
        if ( TheTimer->getAccuTime() > mAutoMoveInfo->mAutoInitTime + mAutoMoveInfo->mAutoLifeTime )
        {
            dSafeDelete ( mAutoMoveInfo );
            mAutoMove = false;
            mAutoTrun = false;
            return;
        }
    }
    if ( mAutoMove )
        updateAutoTranslate();
    if ( mAutoTrun )
        updateAutoMoveRotation();
}

const char* GComponentTrans::getComponentName()
{
    throw std::exception ( "The method or operation is not implemented." );
}

void GComponentTrans::registerAllProperty()
{
    __RegisterProperty ( mMatLocal.mTranslate.x );
    __RegisterProperty ( mMatLocal.mTranslate.y );
    __RegisterProperty ( mMatLocal.mTranslate.z );

    __RegisterProperty ( mDegreeYaw );
    __RegisterProperty ( mDegreePitch );
    __RegisterProperty ( mDegreeRoll );
    //__RegisterProperty ( mMatLocal.mDir.x );
    //__RegisterProperty ( mMatLocal.mDir.y );
    //__RegisterProperty ( mMatLocal.mDir.z );

    //__RegisterProperty ( mMatLocal.mRight.x );
    //__RegisterProperty ( mMatLocal.mRight.y );
    //__RegisterProperty ( mMatLocal.mRight.z );

    //__RegisterProperty ( mMatLocal.mUpon.x );
    //__RegisterProperty ( mMatLocal.mUpon.y );
    //__RegisterProperty ( mMatLocal.mUpon.z );

    //__RegisterProperty ( mBodyRote.x );
    //__RegisterProperty ( mBodyRote.y );
    //__RegisterProperty ( mBodyRote.z );
}

void GComponentTrans::moveTo ( const GMatrix& target, DWORD millSeconds )
{
    dSafeDelete ( mAutoMoveInfo );
    mAutoMoveInfo = new GAutoMoveInfo;
    mAutoMoveInfo->mAutoInitTime = TheTimer->getAccuTime();
    mAutoMoveInfo->mAutoLifeTime = millSeconds;

    D3DXQuaternionRotationMatrix ( &mAutoMoveInfo->mAutoTargetRotation, ( const D3DXMATRIX* ) &target );
    D3DXQuaternionRotationMatrix ( &mAutoMoveInfo->mAutoInitRotation, ( const D3DXMATRIX* ) &mMatWorld );

    mAutoMoveInfo->mAutoTargetTranslate = target.mTranslate;
    mAutoMoveInfo->mAutoInitTranslate = mMatWorld.mTranslate;

    mAutoTrun = true;
    mAutoMove = true;
}

void GComponentTrans::updateAutoTranslate()
{
    dGetVector3Ease ( mMatLocal.mTranslate
                     , &mAutoMoveInfo->mAutoInitTranslate
                     , &mAutoMoveInfo->mAutoTargetTranslate
                     , mAutoMoveInfo->getElpaseTime()
                     , mAutoMoveInfo->mAutoLifeTime
                   );
}

void GComponentTrans::updateAutoMoveRotation()
{
    CXASSERT ( mAutoMoveInfo->getElpaseTime() <= mAutoMoveInfo->mAutoLifeTime );
    D3DXQUATERNION now;
    D3DXQuaternionSlerp ( &now
                          , &mAutoMoveInfo->mAutoInitRotation
                          , &mAutoMoveInfo->mAutoTargetRotation
                          , ( float ) ( mAutoMoveInfo->getElpaseTime() ) / mAutoMoveInfo->mAutoLifeTime
                        );
    D3DXMATRIX mat;
    dGetMatixFromQuation ( mat, now );
    setRotation ( mat );
}

void GComponentTrans::setRotation ( D3DXMATRIX& mat )
{
    mMatLocal.mRight.x = mat._11;
    mMatLocal.mRight.y = mat._12;
    mMatLocal.mRight.z = mat._13;

    mMatLocal.mUpon.x = mat._21;
    mMatLocal.mUpon.y = mat._22;
    mMatLocal.mUpon.z = mat._23;

    mMatLocal.mDir.x = mat._31;
    mMatLocal.mDir.y = mat._32;
    mMatLocal.mDir.z = mat._33;
}

void GComponentTrans::set()
{
    D9Device->GetDvc()->SetTransform ( D3DTS_WORLD, ( const D3DXMATRIX* ) &mMatWorld );
}


void GComponentTrans::updateWorld ( const GMatrix& parentWorldMat )
{
    parentWorldMat.product ( mMatWorld, mMatLocal );
}

void GComponentTrans::setTranslate ( float x, float y, float z )
{
    mMatLocal.mTranslate.x = x;
    mMatLocal.mTranslate.y = y;
    mMatLocal.mTranslate.z = z;
    noitifyWorldNeedUpdate();
}

void GComponentTrans::setYawPitchRow ( float yDegree, float pDegree, float rDegree )
{
    mDegreeYaw = yDegree;
    mDegreePitch = pDegree;
    mDegreeRoll = rDegree;
    noitifyWorldNeedUpdate();
}

void GComponentTrans::updateYawPitchRowRotation()
{
    D3DXMATRIX mat;
    D3DXMatrixRotationYawPitchRoll ( &mat, dDegreeToAngle ( mDegreeYaw ), dDegreeToAngle ( mDegreePitch ), dDegreeToAngle ( mDegreeRoll ) );
    setRotation ( mat );
}

void GComponentTrans::onPropertyChangeEnd ( void* cur )
{
    if (
        cur == &mDegreePitch
        || cur == &mDegreeYaw
        || cur == &mDegreeRoll )
    {
        updateYawPitchRowRotation();
        noitifyWorldNeedUpdate();
    }
    else if ( cur == &mMatLocal.mTranslate.x
              || cur == &mMatLocal.mTranslate.y
              || cur == &mMatLocal.mTranslate.z )
    {
        noitifyWorldNeedUpdate();
    }
}

void GComponentTrans::updateDirUponRight2YawPitchRoll()
{
    float yaw, pitch, roll;
    dGetYawPitchRollFromMatrix ( ( const D3DXMATRIX& ) mMatLocal, yaw, pitch, roll );
    mDegreeYaw = dAgngleToDegree ( yaw );
    mDegreePitch = dAgngleToDegree ( pitch );
    mDegreeRoll = dAgngleToDegree ( roll );
}

void GComponentTrans::noitifyWorldNeedUpdate()
{
    mNeedUpdate = true;
}


















//void CXPosition::Update( float fPass,void *pMap )
//{
//	if(mbIsAtMap || pMap==NULL)
//	{
//		return;
//	}
//
//	mbForceOnMap=false;
//
//	mbCanMoveStep=false;
//
//	CXMap *pmap=(CXMap*)pMap;
//
//	mvSpeed+=D3DXVECTOR3(ZEROFLOAT,-GRAVITY,ZEROFLOAT)*fPass/1000.0f;
//
//	D3DXVECTOR3 vDelta=mvSpeed*fPass/1000.0f;
//
//	D3DXVECTOR3 vNewPos=mvPos+vDelta;
//
//	D3DXVECTOR4 vOrigin(mvPos.x,mvPos.y,mvPos.z,1);
//	D3DXVECTOR4 vDir(mvSpeed.x,mvSpeed.y,mvSpeed.z,0);
//
//	bool bHit=pmap->CheckInsect(vOrigin,vDir,true);
//
//	float fDistence=pmap->mInsectInfo.fDist;
//
//	if (bHit && fDistence<D3DXVec3Length(&vDelta))
//	{
//
//		int nIndex=0;
//
//		if (pmap->IsBlock(pmap->mInsectInfo.vHitPos,&nIndex))
//		{
//			float fy=pmap->mInsectInfo.vHitPos.y+pmap->mppStaticObj[nIndex].fBlockHeight;
//
//			mvPos=D3DXVECTOR3(pmap->mInsectInfo.vHitPos.x,fy,pmap->mInsectInfo.vHitPos.z);
//		}
//		else
//		{
//			mvPos=pmap->mInsectInfo.vHitPos;
//		}
//
//
//		mbIsAtMap=true;
//
//		mbJump=false;
//
//		mbCanMoveStep=true;
//
//		mbBack=false;
//
//		mvSpeed=ZEROVECTOR3;
//
//		mbForceOnMap=true;
//	}
//	else
//	{
//		mvPos=vNewPos;
//	}
//
//}

IntersectInfo::IntersectInfo()
{
    dwFaceIndex = 0;
    u = ZEROFLOAT;
    v = ZEROFLOAT;
    fDist = ZEROFLOAT;
    pObj = NULL;
    vNormal = D3DXVECTOR3 ( ZEROFLOAT, ZEROFLOAT, ZEROFLOAT );
    vHitPos = D3DXVECTOR3 ( ZEROFLOAT, ZEROFLOAT, ZEROFLOAT );
}

GAutoMoveInfo::GAutoMoveInfo()
{
    dMemoryZero ( this, sizeof ( *this ) );
}

DWORD GAutoMoveInfo::getElpaseTime()
{
    return TheTimer->getAccuTime() - mAutoInitTime.getMillSecond();
}
