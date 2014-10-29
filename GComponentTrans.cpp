#include "GGameDemoHeader.h"
#include "GComponentTrans.h"
#include "GTimer.h"

GComponentTrans::GComponentTrans ( void )
{

    mbAutoMove = false;

    mvZoom = D3DXVECTOR3 ( 1.0f, 1.0f, 1.0f );

    mvLastPos = ZEROVECTOR3;
    mTranslate = ZEROVECTOR3;
    mRight = D3DXVECTOR3 ( 1.0f, 0.0f, 0.0f );
    mUpon = D3DXVECTOR3 ( 0.0f, 1.0f, 0.0f );
    mDir = D3DXVECTOR3 ( 0.0f, 0.0f, 1.0f );

    mfSpeedTrun = DEAULT_SPEED_TRUN;
    mfSpeedMove = DEFAULT_SPEED_MOVE;

    mvSpeed = ZEROVECTOR3;

    mvBodyPass = ZEROVECTOR3;
    mBodyRote = ZEROVECTOR3;

    mvTargetPos = ZEROVECTOR3;

    mvTargetDir = ZEROVECTOR3;

    mbAutoMoveWithTarget = false;

    mbAutoTrunWithTarget = false;

    mbJump = false;

    mbCanMoveStep = true;

    mbBack = false;
}

GComponentTrans::~GComponentTrans ( void )
{

}

D3DXMATRIX GComponentTrans::GetWorldMatrix ( bool bForTrans )
{
    D3DXMATRIX matWorld;    //ÊÀ½ç¾ØÕó

    D3DXMATRIX matScale;	    //Ëõ·Å¾ØÕó

    D3DXMATRIX matTranslation;	//Æ½ÒÆ¾ØÕó

    D3DXMATRIX matRotation;  //Ðý×ª¾ØÕó

    D3DXMatrixIdentity ( &matWorld );

    if ( !bForTrans )
    {
        //¼ÆËãËõ·Å¾ØÕó
        D3DXMatrixScaling ( &matScale, mvZoom.x, mvZoom.y, mvZoom.z );
        matWorld *= matScale;

        //¼ÆËãÐý×ª¾ØÕó
        D3DXMatrixRotationYawPitchRoll ( &matRotation, mBodyRote.y, mBodyRote.x, mBodyRote.z );
        matWorld *= matRotation;

    }

    D3DXVec3Normalize ( &mRight, &mRight );
    D3DXVec3Normalize ( &mDir, &mDir );
    D3DXVec3Normalize ( &mUpon, &mUpon );

    matWorld *= D3DXMATRIX (
                    mRight.x, mRight.y, mRight.z, 0,
                    mUpon.x, mUpon.y, mUpon.z, 0,
                    mDir.x, mDir.y, mDir.z, 0,
                    0.0f, 0.0f, 0.0f, 1.0f
                );

    //¼ÆËãÆ½ÒÆ¾ØÕó
    if ( bForTrans )
    {
        D3DXMatrixTranslation ( &matTranslation, mTranslate.x, mTranslate.y, mTranslate.z );
    }
    else
    {
        D3DXMatrixTranslation ( &matTranslation, mTranslate.x + mvBodyPass.x * mvZoom.x, mTranslate.y + mvBodyPass.y * mvZoom.y, mTranslate.z + mvBodyPass.z * mvZoom.z );
    }

    matWorld *= matTranslation;

    return matWorld;
}


D3DXVECTOR3 GComponentTrans::MoveStep ( float fPass )
{

    if ( !mbCanMoveStep )
    {
        return ZEROVECTOR3;
    }

    float fLen = mfSpeedMove * fPass;

    D3DXVec3Normalize ( &mDir, &mDir );

    mvLastPos = mTranslate;

    mTranslate += fLen * mDir;

    return mTranslate;
}

D3DXVECTOR3 GComponentTrans::TrunStepLeftRight ( float fPara )
{
    D3DXMATRIX matRotation;
    D3DXVECTOR3 vRotation ( mUpon );
    D3DXMatrixRotationAxis ( &matRotation, &vRotation, fPara );

    D3DXVec3TransformCoord ( &mRight, &mRight, &matRotation );
    D3DXVec3TransformCoord ( &mDir, &mDir, &matRotation );

    D3DXVec3Normalize ( &mRight, &mRight );
    D3DXVec3Normalize ( &mUpon, &mUpon );
    D3DXVec3Normalize ( &mDir, &mDir );

    return mDir;
}

D3DXVECTOR3 GComponentTrans::TrunWithRight ( float fPara )
{
    D3DXMATRIX matRotation;
    D3DXVECTOR3 vRotation ( mRight );
    D3DXMatrixRotationAxis ( &matRotation, &vRotation, fPara );

    D3DXVec3TransformCoord ( &mUpon, &mUpon, &matRotation );
    D3DXVec3TransformCoord ( &mDir, &mDir, &matRotation );
    D3DXVec3Cross ( &mRight, &mUpon, &mDir );

    D3DXVec3Normalize ( &mRight, &mRight );
    D3DXVec3Normalize ( &mUpon, &mUpon );
    D3DXVec3Normalize ( &mDir, &mDir );

    return mDir;

}


D3DXVECTOR3 GComponentTrans::SetDirWithUpon ( D3DXVECTOR3 vUpon )
{

    D3DXVec3Cross ( &mRight, &vUpon, &mDir );

    D3DXVec3Cross ( &mDir, &mRight, &vUpon );

    D3DXVec3Normalize ( &mDir, &mDir );
    D3DXVec3Normalize ( &mRight, &mRight );
    D3DXVec3Normalize ( &mUpon, &vUpon );

    return mDir;
}

D3DXVECTOR3 GComponentTrans::TrunStepLeftRightWithUp ( float fpara )
{
    D3DXMATRIX matRotation;
    D3DXVECTOR3 vRotation ( ZEROFLOAT, 1.0f, ZEROFLOAT );
    D3DXMatrixRotationAxis ( &matRotation, &vRotation, fpara );

    D3DXVec3TransformCoord ( &mUpon, &mUpon, &matRotation );
    D3DXVec3TransformCoord ( &mDir, &mDir, &matRotation );
    D3DXVec3Cross ( &mRight, &mUpon, &mDir );

    D3DXVec3Normalize ( &mRight, &mRight );
    D3DXVec3Normalize ( &mUpon, &mUpon );
    D3DXVec3Normalize ( &mDir, &mDir );

    return mDir;
}

int GComponentTrans::Jump()
{
    if ( !mbJump )
    {
        mbAutoMove = true;

        mbCanMoveStep = false;

        mvSpeed.y = JUMP_HEIGHT / 0.5f / JUMP_TIME;

        toggle ( mbJump );
    }

    return TRUE_INT;

}

D3DXVECTOR3 GComponentTrans::MoveToPos ( D3DXVECTOR3 vTarget )
{
    mbAutoMoveWithTarget = true;

    mvTargetPos = vTarget;

    return mvTargetPos;
}


D3DXMATRIX GComponentTrans::GetRotationMatrix ( bool bForTrans/*=false*/ )
{
    D3DXMATRIX matRotation;
    D3DXMatrixIdentity ( &matRotation );

    if ( !bForTrans )
    {
        D3DXMatrixRotationYawPitchRoll ( &matRotation, mBodyRote.y, mBodyRote.x, mBodyRote.z );
    }
    matRotation *= D3DXMATRIX (
                       mRight.x, mRight.y, mRight.z, 0,
                       mUpon.x, mUpon.y, mUpon.z, 0,
                       mDir.x, mDir.y, mDir.z, 0,
                       0.0f, 0.0f, 0.0f, 1.0f
                   );
    return matRotation;
}



D3DXMATRIX GComponentTrans::GetScaleMatrix ( bool bForTrans/*=false*/ )
{
    D3DXMATRIX matScaling = NORMALMATRIX;

    if ( !bForTrans )
    {
        D3DXMatrixScaling ( &matScaling, mvZoom.x, mvZoom.y, mvZoom.z );
    }

    return matScaling;
}

D3DXMATRIX GComponentTrans::GetTransLation ( bool bForTrans/*=false*/ )
{
    D3DXMATRIX matTranslation = NORMALMATRIX;

    if ( bForTrans )
    {
        D3DXMatrixTranslation ( &matTranslation, mTranslate.x, mTranslate.y, mTranslate.z );
    }
    else
    {
        D3DXMatrixTranslation ( &matTranslation, mTranslate.x + mvBodyPass.x * mvZoom.x, mTranslate.y + mvBodyPass.y * mvZoom.y, mTranslate.z + mvBodyPass.z * mvZoom.z );
    }

    return matTranslation;
}


D3DXVECTOR3 GComponentTrans::TrunToDir ( D3DXVECTOR3 vTargetDir )
{

    mbAutoTrunWithTarget = true;

    D3DXVec3Normalize ( &mvTargetDir, &vTargetDir );

    return mvTargetDir;
}

void GComponentTrans::Update(  )
{
    float fTime = TheTimer->getFrameTimems() / 1000.0f;


    if ( !mbAutoMove )
    {
        return;
    }

    mvLastPos = mTranslate;


    mvSpeed += D3DXVECTOR3 ( ZEROFLOAT, -GRAVITY, ZEROFLOAT ) * fTime;

    D3DXVECTOR3 vDelta = mvSpeed * TheTimer->getFrameTimems() / 1000.0f;

    mTranslate += vDelta;
}

const char* GComponentTrans::GetComponentName()
{
    throw std::exception ( "The method or operation is not implemented." );
}

void GComponentTrans::registerAllProperty()
{
    __RegisterProperty ( mTranslate.x );
    __RegisterProperty ( mTranslate.y );
    __RegisterProperty ( mTranslate.z );

	__RegisterProperty ( mDir.x );
	__RegisterProperty ( mDir.y );
	__RegisterProperty ( mDir.z );

	__RegisterProperty ( mRight.x );
	__RegisterProperty ( mRight.y );
	__RegisterProperty ( mRight.z );

	__RegisterProperty ( mUpon.x );
	__RegisterProperty ( mUpon.y );
	__RegisterProperty ( mUpon.z );

    //__RegisterProperty ( mBodyRote.x );
    //__RegisterProperty ( mBodyRote.y );
    //__RegisterProperty ( mBodyRote.z );
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
