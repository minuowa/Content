#include "GGameDemoHeader.h"
#include "GComponentTrans.h"
#include "GTimer.h"

GComponentTrans::GComponentTrans ( void )
{

    mbAutoMove = false;

    mvZoom = D3DXVECTOR3 ( 1.0f, 1.0f, 1.0f );

    mvLastPos = ZEROVECTOR3;
    mTranslate = ZEROVECTOR3;
    mvRight = D3DXVECTOR3 ( 1.0f, 0.0f, 0.0f );
    mvUpon = D3DXVECTOR3 ( 0.0f, 1.0f, 0.0f );
    mvDir = D3DXVECTOR3 ( 0.0f, 0.0f, 1.0f );

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
    D3DXMATRIX matWorld;    //�������

    D3DXMATRIX matScale;	    //���ž���

    D3DXMATRIX matTranslation;	//ƽ�ƾ���

    D3DXMATRIX matRotation;  //��ת����

    D3DXMatrixIdentity ( &matWorld );

    if ( !bForTrans )
    {
        //�������ž���
        D3DXMatrixScaling ( &matScale, mvZoom.x, mvZoom.y, mvZoom.z );
        matWorld *= matScale;

        //������ת����
        D3DXMatrixRotationYawPitchRoll ( &matRotation, mBodyRote.y, mBodyRote.x, mBodyRote.z );
        matWorld *= matRotation;

    }

    D3DXVec3Normalize ( &mvRight, &mvRight );
    D3DXVec3Normalize ( &mvDir, &mvDir );
    D3DXVec3Normalize ( &mvUpon, &mvUpon );

    matWorld *= D3DXMATRIX (
                    mvRight.x, mvRight.y, mvRight.z, 0,
                    mvUpon.x, mvUpon.y, mvUpon.z, 0,
                    mvDir.x, mvDir.y, mvDir.z, 0,
                    0.0f, 0.0f, 0.0f, 1.0f
                );

    //����ƽ�ƾ���
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

    D3DXVec3Normalize ( &mvDir, &mvDir );

    mvLastPos = mTranslate;

    mTranslate += fLen * mvDir;

    return mTranslate;
}

D3DXVECTOR3 GComponentTrans::TrunStepLeftRight ( float fPara )
{
    D3DXMATRIX matRotation;
    D3DXVECTOR3 vRotation ( mvUpon );
    D3DXMatrixRotationAxis ( &matRotation, &vRotation, fPara );

    D3DXVec3TransformCoord ( &mvRight, &mvRight, &matRotation );
    D3DXVec3TransformCoord ( &mvDir, &mvDir, &matRotation );

    D3DXVec3Normalize ( &mvRight, &mvRight );
    D3DXVec3Normalize ( &mvUpon, &mvUpon );
    D3DXVec3Normalize ( &mvDir, &mvDir );

    return mvDir;
}

D3DXVECTOR3 GComponentTrans::TrunWithRight ( float fPara )
{
    D3DXMATRIX matRotation;
    D3DXVECTOR3 vRotation ( mvRight );
    D3DXMatrixRotationAxis ( &matRotation, &vRotation, fPara );

    D3DXVec3TransformCoord ( &mvUpon, &mvUpon, &matRotation );
    D3DXVec3TransformCoord ( &mvDir, &mvDir, &matRotation );
    D3DXVec3Cross ( &mvRight, &mvUpon, &mvDir );

    D3DXVec3Normalize ( &mvRight, &mvRight );
    D3DXVec3Normalize ( &mvUpon, &mvUpon );
    D3DXVec3Normalize ( &mvDir, &mvDir );

    return mvDir;

}


D3DXVECTOR3 GComponentTrans::SetDirWithUpon ( D3DXVECTOR3 vUpon )
{

    D3DXVec3Cross ( &mvRight, &vUpon, &mvDir );

    D3DXVec3Cross ( &mvDir, &mvRight, &vUpon );

    D3DXVec3Normalize ( &mvDir, &mvDir );
    D3DXVec3Normalize ( &mvRight, &mvRight );
    D3DXVec3Normalize ( &mvUpon, &vUpon );

    return mvDir;
}

D3DXVECTOR3 GComponentTrans::TrunStepLeftRightWithUp ( float fpara )
{
    D3DXMATRIX matRotation;
    D3DXVECTOR3 vRotation ( ZEROFLOAT, 1.0f, ZEROFLOAT );
    D3DXMatrixRotationAxis ( &matRotation, &vRotation, fpara );

    D3DXVec3TransformCoord ( &mvUpon, &mvUpon, &matRotation );
    D3DXVec3TransformCoord ( &mvDir, &mvDir, &matRotation );
    D3DXVec3Cross ( &mvRight, &mvUpon, &mvDir );

    D3DXVec3Normalize ( &mvRight, &mvRight );
    D3DXVec3Normalize ( &mvUpon, &mvUpon );
    D3DXVec3Normalize ( &mvDir, &mvDir );

    return mvDir;
}

int GComponentTrans::Jump()
{
    if ( !mbJump )
    {
        mbAutoMove = true;

        mbCanMoveStep = false;

        mvSpeed.y = JUMP_HEIGHT / 0.5f / JUMP_TIME;

        Toggle ( mbJump );
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
                       mvRight.x, mvRight.y, mvRight.z, 0,
                       mvUpon.x, mvUpon.y, mvUpon.z, 0,
                       mvDir.x, mvDir.y, mvDir.z, 0,
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
    float fTime = TIMER.GetFrameTimems() / 1000.0f;


    if ( !mbAutoMove )
    {
        return;
    }

    mvLastPos = mTranslate;


    mvSpeed += D3DXVECTOR3 ( ZEROFLOAT, -GRAVITY, ZEROFLOAT ) * fTime;

    D3DXVECTOR3 vDelta = mvSpeed * TIMER.GetFrameTimems() / 1000.0f;

    mTranslate += vDelta;
}

const char* GComponentTrans::GetComponentName()
{
    throw std::exception ( "The method or operation is not implemented." );
}

void GComponentTrans::RegisterAll()
{
    __RegisterProperty ( mTranslate.x );
    __RegisterProperty ( mTranslate.y );
    __RegisterProperty ( mTranslate.z );

    __RegisterProperty ( mBodyRote.x );
    __RegisterProperty ( mBodyRote.y );
    __RegisterProperty ( mBodyRote.z );
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
