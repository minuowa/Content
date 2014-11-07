#include "GGameDemoHeader.h"
#include "GAnimMeshObj.h"
#include "XCursor.h"

#include "Particles.h"
#include "GAnimationResource.h"
#include "GComponentMesh.h"
#include "GResourceManager.h"
#include "XSingleton.h"
#include "GTimer.h"
#include "GD8Input.h"

GAnimMeshObj::GAnimMeshObj ( void )
	: mResource ( 0 )
	, mCloneAnimationController ( 0 )
{
	ZeroMemory ( m_sTextureName, sizeof ( m_sTextureName ) );

	mpAmmo = NULL;

	LnNowBoneInfoCount = -1;

	mbPlayDone = false;

	mpAnimSet = NULL;

	attachComponent ( eComponentType_Mesh, false );
}

GAnimMeshObj::~GAnimMeshObj ( void )
{
	dSafeRelease ( mCloneAnimationController );
}





VOID GAnimMeshObj::UpdateFrameMatrices ( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix )
{
	if ( pFrameBase == NULL )
	{
		return;
	}

	D3DXFrameEX *pFrameEx = ( D3DXFrameEX* ) pFrameBase;

	if ( pParentMatrix != NULL )
	{
		D3DXMatrixMultiply ( &pFrameEx->matCombinedTransformation, &pFrameEx->TransformationMatrix, pParentMatrix );
	}
	else
	{
		pFrameEx->matCombinedTransformation = pFrameEx->TransformationMatrix;
	}

	if ( pFrameEx->pFrameSibling != NULL )
	{
		UpdateFrameMatrices ( pFrameEx->pFrameSibling, pParentMatrix );
	}

	if ( pFrameEx->pFrameFirstChild != NULL )
	{
		UpdateFrameMatrices ( pFrameEx->pFrameFirstChild, &pFrameEx->matCombinedTransformation );
	}
}

void GAnimMeshObj::DrawMeshContainer ( D3DXMESHCONTAINER *pMeshContainerBase, D3DXFRAME *pFrameBase )
{
	if ( pMeshContainerBase == NULL || pFrameBase == NULL )
	{
		return;
	}

	UINT iMatrixIndex = 0;

	D3DXBONECOMBINATION *pBoneComb = NULL;

	D3DXMATRIX matTmp;


	D3DXMeshContainerEX *pMeshContainerEx = ( D3DXMeshContainerEX* ) pMeshContainerBase;
	D3DXFrameEX *pFrameEx = ( D3DXFrameEX* ) pFrameBase;

	D3DCAPS9 d3dCap;
	ZeroMemory ( &d3dCap, sizeof ( d3dCap ) );
	D9DEVICE->GetDvc()->GetDeviceCaps ( &d3dCap );

	if ( pMeshContainerEx->pBoneCombinationBufffer != NULL )
	{
		pBoneComb = ( D3DXBONECOMBINATION* ) ( pMeshContainerEx->pBoneCombinationBufffer->GetBufferPointer() );
	}


	D9DEVICE->OpenAllLight ( true );

	DWORD dwAmbient = 0;
	D9DEVICE->GetDvc()->GetRenderState ( D3DRS_AMBIENT, &dwAmbient );

	POINT pt = INPUTSYSTEM.GetMousePoint();

	bool bHit = false;

	if ( INPUTSYSTEM.isPressingButton ( eButtonType_LeftButton ) )
	{
		if ( pMeshContainerEx != NULL && pMeshContainerEx->MeshData.pMesh != NULL && !m_bHit )
		{
			m_bHit = Pick ( pMeshContainerEx->MeshData.pMesh, pt );

			if ( m_bHit )
			{
				toggle ( getTrans().mCanMoveStep );
			}
		}
	}

	for ( DWORD iAttr = 0; iAttr < pMeshContainerEx->NumAttributeGroups; iAttr++	)
	{
		if ( d3dCap.MaxVertexBlendMatrices >= pMeshContainerEx->NumInfl )
		{
			D9DEVICE->GetDvc()->SetRenderState ( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );

			for ( DWORD i = 0; i < pMeshContainerEx->NumInfl; ++i )
			{
				iMatrixIndex = pBoneComb[iAttr].BoneId[i];

				if ( iMatrixIndex != UINT_MAX )
				{
					D3DXMatrixMultiply
						(
						&matTmp,
						&pMeshContainerEx->pBoneOffsetMatrices[iMatrixIndex],
						pMeshContainerEx->ppBoneMatrixPtrs[iMatrixIndex]
					);

					D9DEVICE->GetDvc()->SetTransform ( D3DTS_WORLDMATRIX ( i ), &matTmp );
				}
			}

			D9DEVICE->GetDvc()->SetRenderState ( D3DRS_VERTEXBLEND, pMeshContainerEx->NumInfl - 1 );

			D9DEVICE->GetDvc()->SetMaterial ( &pMeshContainerEx->pMaterials[pBoneComb[iAttr].AttribId].MatD3D );

			if ( m_bHit )
			{
				gCursor.SetNowCursor ( curGrasp );
				D9DEVICE->GetDvc()->SetTexture ( 0, NULL );
			}
			else
			{
				gCursor.SetNowCursor ( curNormal );
				D9DEVICE->GetDvc()->SetTexture ( 0, pMeshContainerEx->ppTexture[pBoneComb[iAttr].AttribId] );
			}


			DWORD dwLight = 0;

			D9DEVICE->GetDvc()->GetRenderState ( D3DRS_LIGHTING, &dwLight );

			pMeshContainerEx->MeshData.pMesh->DrawSubset ( iAttr );

		}

		D9DEVICE->GetDvc()->SetRenderState ( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );

		D9DEVICE->GetDvc()->SetRenderState ( D3DRS_VERTEXBLEND, FALSE );
	}

	D9DEVICE->OpenAllLight ( false );

}

void GAnimMeshObj::DrawFrame ( D3DXFRAME *pFrameBase )
{
	if ( pFrameBase == NULL )
	{
		return ;
	}

	D3DXMESHCONTAINER *pMeshContainer = pFrameBase->pMeshContainer;

	while ( pMeshContainer != NULL )
	{
		DrawMeshContainer ( pMeshContainer, pFrameBase );

		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	if ( pFrameBase->pFrameSibling != NULL )
	{
		DrawFrame ( pFrameBase->pFrameSibling );
	}

	if ( pFrameBase->pFrameFirstChild != NULL )
	{
		DrawFrame ( pFrameBase->pFrameFirstChild );
	}

}

eObjAnimState GAnimMeshObj::SetState ( eObjAnimState oas, bool bBack )
{
	ID3DXAnimationSet *pAS = NULL;

	HRESULT hr = S_FALSE;

	if ( oas == m_ObjAnimState )
	{
		return m_ObjAnimState;
	}

	getTrans().mBack = bBack;

	if ( mCloneAnimationController == NULL )
	{
		return oasNULL;
	}

	if ( m_ObjAnimState == oasAttack || oasRunAttack == m_ObjAnimState )
	{
		if ( !mbPlayDone )
		{
			return m_ObjAnimState;
		}
	}

	m_ObjAnimState = oas;

	//获取动作集
	switch ( oas )
	{
	case oasDead:

		mCloneAnimationController->GetAnimationSetByName ( "Death", &pAS );

		break;

	case oasMoving:

		mCloneAnimationController->GetAnimationSetByName ( "Move", &pAS );

		break;

	case oasStandBy:

		mCloneAnimationController->GetAnimationSetByName ( "Stand_By", &pAS );

		break;

	case oasBeAttack:

		mCloneAnimationController->GetAnimationSetByName ( "Be_Attacked", &pAS );
		break;

	case oasRunAttack:

		mCloneAnimationController->GetAnimationSetByName ( "RunAttact", &pAS );
		break;

	case oasAttack:

		mCloneAnimationController->GetAnimationSetByName ( "Attack", &pAS );

		SAFED_ELETE ( mpAmmo );

		//mpAmmo = new CAmmoParticles;

		//mpAmmo->Create( this, D9DEVICE, 4000, mForceMap );

		//mpAmmo->Shoot();

		break;
	}

	if ( pAS != NULL )
	{
		//设置动作速度

		if ( getTrans().mBack )
		{
			mCloneAnimationController->SetTrackSpeed ( 0, -1 );
		}
		else
		{
			mCloneAnimationController->SetTrackSpeed ( 0, 1 );
		}


		//设置动画集

		hr = mCloneAnimationController->SetTrackAnimationSet ( 0, pAS );

		//设置动画播放起点

		hr = mCloneAnimationController->SetTrackPosition ( 0, 0 );

		SAFERELEASE ( mpAnimSet );

		mdwOldAnimSetFrame = 0;
		mdwCurAnimSetFrame = 0;

		mbPlayDone = false;

		mpAnimSet = pAS;

	}

	return m_ObjAnimState;
}



bool GAnimMeshObj::Pick ( ID3DXMesh *pMesh, POINT pt )
{
	HRESULT hr = S_FALSE;

	BOOL bHit = false;

	D3DXMATRIX matProj, matView, matWorld;
	D3DXMATRIX matTmp;

	D9DEVICE->GetDvc()->GetTransform ( D3DTS_PROJECTION, &matProj );
	D9DEVICE->GetDvc()->GetTransform ( D3DTS_VIEW, &matView );
	D9DEVICE->GetDvc()->GetTransform ( D3DTS_WORLD, &matWorld );

	D3DXVECTOR4 vOrigin ( ZEROFLOAT, ZEROFLOAT, ZEROFLOAT, 1.0f );
	D3DXVECTOR4 vDir;

	//注意此处的2.0f与2的区别
	vDir.x = ( 2.0f * pt.x / D9DEVICE->mWidth - 1 ) / matProj._11;
	vDir.y = - ( 2.0f * pt.y / D9DEVICE->mHeight - 1 ) / matProj._22;
	vDir.z = 1.0f;
	vDir.w = 0.0f;

	matTmp = matWorld * matView;
	D3DXMatrixInverse ( &matTmp, NULL, &matTmp );

	D3DXVec4Transform ( &vOrigin, &vOrigin, &matTmp );
	D3DXVec4Transform ( &vDir, &vDir, &matTmp );

	hr = D3DXIntersect ( pMesh, ( D3DXVECTOR3* ) &vOrigin, ( D3DXVECTOR3* ) &vDir, &bHit, NULL, NULL, NULL, NULL, NULL, NULL );

	if ( FAILED ( hr ) )
	{
		return false;
	}

	return ( BOOL ) bHit;
}





D3DXMATRIX GAnimMeshObj::GetWorldMatrixByBone ( char *sBoneName, bool bForTrans/*=false*/ )
{
	if ( IsStrEmpty ( sBoneName ) )
	{
		return GNode::GetWorldMatrixByBone ( sBoneName, bForTrans );
	}
	CXASSERT ( mResource );
	GBoneLinker *pBoneInfo = mResource->GetBoneInfo ( sBoneName );

	if ( pBoneInfo != NULL )
	{
		return * ( pBoneInfo->mTransform );
	}

	return GNode::GetWorldMatrixByBone ( sBoneName, bForTrans );
}


bool GAnimMeshObj::recreate()
{
	if ( !__super::recreate() )
	{
		return false;
	}
	GComponentMesh* componentMesh = ( GComponentMesh* ) mComponentOwner.getComponent ( eComponentType_Mesh );
	CXASSERT_RESULT_FALSE ( componentMesh );
	mResource = GAnimationManager::GetSingleton().getResource ( componentMesh->MeshFile().c_str() );
	CXASSERT_RETURN_FALSE ( mResource );
	ID3DXAnimationController* orginal = mResource->mAnimationController;
	if ( orginal )
	{
		CXASSERT_RESULT_FALSE ( orginal->CloneAnimationController
			(
			orginal->GetMaxNumAnimationOutputs(), orginal->GetMaxNumAnimationSets(),
			orginal->GetMaxNumTracks(), orginal->GetMaxNumEvents(), &mCloneAnimationController
			)
			);
	}

	//SetState ( oasStandBy, false );

	return true;
}

void GAnimMeshObj::update()
{
	return;
}


bool GAnimMeshObj::render()
{
	if ( !__super::render() )
		return false;
	updateWorldInfo();
	if ( mResource && mResource->mFrameRoot )
	{
		DrawFrame ( mResource->mFrameRoot );
		if ( mpAmmo != NULL )
		{
			mpAmmo->render();

			if ( !mpAmmo->getTrans().mAutoMove )
			{
				SAFED_ELETE ( mpAmmo );
			}
		}

		m_bHit = false;
	}
	return true;
}

void GAnimMeshObj::setMediaFile ( const char* file )
{
	GComponentMesh* componentMesh = ( GComponentMesh* ) mComponentOwner.getComponent ( eComponentType_Mesh );
	CXASSERT_RETURN ( componentMesh );
	componentMesh->MeshFile ( file );
}

void GAnimMeshObj::updateWorldInfo()
{
	if ( mCloneAnimationController )
	{
		mCloneAnimationController->AdvanceTime ( TheTimer->getFrameTimeSec(), NULL );

		if ( mpAnimSet != NULL )
		{
			D3DXTRACK_DESC trackDesc;

			mCloneAnimationController->GetTrackDesc ( 0, &trackDesc );

			double dbPassTime = mpAnimSet->GetPeriodicPosition ( trackDesc.Position );

			mdwCurAnimSetFrame = dbPassTime * 300000;

			if ( mdwCurAnimSetFrame < mdwOldAnimSetFrame )
			{
				mbPlayDone = true;
			}

			mdwOldAnimSetFrame = mdwCurAnimSetFrame;
		}
	}
	if ( mResource && mResource->mFrameRoot )
	{
		UpdateFrameMatrices ( mResource->mFrameRoot, &_matWorld );
	}
	if ( mpAmmo != NULL )
	{
		mpAmmo->update();
	}
}

