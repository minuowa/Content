#include "GGameDemoHeader.h"
#include "GStillEntity.h"
#include "GComponentTrans.h"
#include "GEffect.h"

#include "GSceneMgr.h"
#include "GGame.h"

GStillEntity::GStillEntity ( void )
    : mMeshBufferNode ( 0 )
{
    mMeshForInsect = nullptr;
}

GStillEntity::~GStillEntity ( void )
{
    dSafeRelease ( mMeshForInsect );
}


bool GStillEntity::render()
{
    if ( !__super::render() )
        return false;

    if ( mMeshBufferNode )
        return mMeshBufferNode->draw();
    return true;
    //D9DEVICE->OpenAllLightEx(m_bLightOn,mfDiffuseIntensity,ZEROFLOAT,m_bUseMatrialColor);

    //DWORD dwAmbient = 0;
    //D9DEVICE->GetDvc()->GetRenderState( D3DRS_AMBIENT, &dwAmbient );
    ////绘制不透明部分

    //gEffect.mD3DEffect->SetBool( gEffect.m_hOpenLight, ( BOOL )mLighting );
    //gEffect.mD3DEffect->SetBool( gEffect.m_hUseMaterialOnly, ( BOOL )m_bUseMatrialColor );

    //UINT iPass = 0;
    //gEffect.mD3DEffect->Begin( &iPass, 0 );


    //gEffect.mD3DEffect->SetTechnique( gEffect.m_Tech );


    //D3DXMATRIX matWorldViewProj = _matWorld * TheSceneMgr->mEye.matView * TheSceneMgr->mEye.matProj;

    //gEffect.mD3DEffect->SetMatrix( gEffect.m_hWorldViewProj, &matWorldViewProj );

    //gEffect.mD3DEffect->SetMatrix( gEffect.m_hWorld, &_matWorld );

    //for ( DWORD i = 0; i < mMeshBufferNode->SubSetCount(); i++ )
    //{
    //    if ( !( mMeshBufferNode->mpMat[i].Diffuse.a < 1.0f ) )
    //    {
    //        //设置网格模型子集的材质和纹理，并且渲染

    //        D3DMATERIAL9 *pTmp = &mMeshBufferNode->mpMat[i];

    //        D3DXVECTOR4 vDiffuse( pTmp->Diffuse.r, pTmp->Diffuse.g, pTmp->Diffuse.b, pTmp->Diffuse.a );
    //        D3DXVECTOR4 vAmbient( pTmp->Ambient.r, pTmp->Ambient.g, pTmp->Ambient.b, pTmp->Ambient.a );

    //        gEffect.mD3DEffect->SetVector( gEffect.m_hMtrlDiffuse, &vDiffuse );
    //        gEffect.mD3DEffect->SetVector( gEffect.m_hMtrlAmbient, &vAmbient );

    //        if ( ppFace != NULL )
    //        {
    //            //Alpha测试:

    //            D9DEVICE->GetDvc()->SetRenderState( D3DRS_ALPHATESTENABLE, true );

    //            D9DEVICE->GetDvc()->SetRenderState( D3DRS_ALPHAREF, 0x00000000 );

    //            D9DEVICE->GetDvc()->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER );

    //            if ( mChangeWhenHit )
    //            {
    //                D9DEVICE->GetDvc()->SetTexture( 0, NULL );
    //            }
    //            else
    //            {
    //                gEffect.mD3DEffect->SetTexture( gEffect.m_hTexture, ppFace[i] );
    //            }
    //        }

    //        gEffect.mD3DEffect->BeginPass( 0 );

    //        mMeshBufferNode->Mesh()->DrawSubset( i );

    //        gEffect.mD3DEffect->EndPass();

    //    }
    //    else
    //    {
    //        continue;
    //    }
    //}

    ////绘制透明部分
    //D9DEVICE->OpenAlphaBlend( true );

    //for ( DWORD i = 0; i < mMeshBufferNode->LnAttrNum; i++ )
    //{
    //    if ( mMeshBufferNode->mpMat[i].Diffuse.a < 1.0f )
    //    {
    //        //D9DEVICE->GetDvc()->SetMaterial(&mpMeshBufferNode->mpMat[i]);

    //        D3DMATERIAL9 *pTmp = &mMeshBufferNode->mpMat[i];

    //        D3DXVECTOR4 vDiffuse( pTmp->Diffuse.r, pTmp->Diffuse.g, pTmp->Diffuse.b, pTmp->Diffuse.a );
    //        D3DXVECTOR4 vAmbient( pTmp->Ambient.r, pTmp->Ambient.g, pTmp->Ambient.b, pTmp->Ambient.a );

    //        gEffect.mD3DEffect->SetVector( gEffect.m_hMtrlDiffuse, &vDiffuse );
    //        gEffect.mD3DEffect->SetVector( gEffect.m_hMtrlAmbient, &vAmbient );

    //        if ( ppFace != NULL )
    //        {
    //            if ( mChangeWhenHit )
    //            {
    //                D9DEVICE->GetDvc()->SetTexture( 0, NULL );
    //            }
    //            else
    //            {
    //                //D9DEVICE->GetDvc()->SetTexture(0,ppFace[i]);
    //                gEffect.mD3DEffect->SetTexture( gEffect.m_hTexture, ppFace[i] );
    //            }
    //        }


    //        gEffect.mD3DEffect->BeginPass( 0 );

    //        //mpMeshBufferNode->mMeshNodeForLod.pMesh[LnLodNowLevel]->DrawSubset(i);
    //        mMeshBufferNode->mpRootMesh->DrawSubset( i );

    //        gEffect.mD3DEffect->EndPass();

    //    }
    //    else
    //    {
    //        continue;
    //    }
    //}

    //gEffect.mD3DEffect->End();

    //D9DEVICE->GetDvc()->SetRenderState( D3DRS_ALPHATESTENABLE, false );

    //D9DEVICE->OpenAlphaBlend( false );

    //D9DEVICE->OpenAllLight( false );
}




ID3DXMesh * GStillEntity::recreateInsectMesh()
{
    dSafeRelease ( mMeshForInsect );

    if ( mMeshBufferNode->getMesh() != NULL )
    {
        mMeshBufferNode->getMesh()->CloneMeshFVF (
            mMeshBufferNode->getMesh()->GetOptions(),
            D3DFVF_XYZ, D9DEVICE->GetDvc(), &mMeshForInsect
        );
    }

    return mMeshForInsect;
}


bool GStillEntity::checkIntersect ( const D3DXVECTOR4& vPos, /*世界坐标系中的点 */ const D3DXVECTOR4& vDir, /*世界坐标系中的向量 */ bool bInsectInfo /*是 裥枰鲎残畔?*/ )
{
    HRESULT hr = S_FALSE;

    //将Pos和Dir转换到物体本地坐标系中
    D3DXMATRIX matWorld = getTrans()->getLocalD3D();
    D3DXMatrixInverse ( &matWorld, NULL, &matWorld );

    D3DXVec4Transform ( ( D3DXVECTOR4 * ) &vDir, ( D3DXVECTOR4 * ) &vDir, &matWorld );
    D3DXVec3Normalize ( ( D3DXVECTOR3* ) &vDir, ( D3DXVECTOR3* ) &vDir );
    D3DXVec4Transform ( ( D3DXVECTOR4 * ) &vPos, ( D3DXVECTOR4 * ) &vPos, &matWorld );

    if ( mMeshForInsect == NULL )
    {
        recreateInsectMesh();
    }

    BOOL bHit = FALSE;

    hr = D3DXIntersect (
             mMeshForInsect, ( D3DXVECTOR3* ) &vPos, ( D3DXVECTOR3* ) &vDir, &bHit,
             &m_InsectInfo.dwFaceIndex, &m_InsectInfo.u, &m_InsectInfo.v, &m_InsectInfo.fDist,
             NULL, NULL
         );

    mNodeState.setBit ( eObjState_Picked, ( bool ) bHit );
    DebugMsgBox ( hr, "碰撞失败！" );

    if ( FAILED ( hr ) )
    {
        return false;
    }

    if ( bInsectInfo && mNodeState[eObjState_Picked] )
    {
        D3DXVECTOR3 v[3];
        DWORD dwIndex[3];

        //先要获取索引缓冲区格式
        LPDIRECT3DINDEXBUFFER9 pI = NULL;
        mMeshForInsect->GetIndexBuffer ( &pI );

        D3DINDEXBUFFER_DESC indexDesc;
        ZeroMemory ( &indexDesc, sizeof ( D3DINDEXBUFFER_DESC ) );

        if ( pI != NULL )
        {
            pI->GetDesc ( &indexDesc );
        }

        if ( indexDesc.Format == D3DFMT_INDEX16 )
        {
            WORD *pIndexData16;

            hr = mMeshForInsect->LockIndexBuffer ( D3DLOCK_READONLY, ( void** ) &pIndexData16 );

            dwIndex[0] = pIndexData16[m_InsectInfo.dwFaceIndex * 3 + 0];
            dwIndex[1] = pIndexData16[m_InsectInfo.dwFaceIndex * 3 + 1];
            dwIndex[2] = pIndexData16[m_InsectInfo.dwFaceIndex * 3 + 2];
        }
        else
        {
            DWORD *pIndexData32;

            hr = mMeshForInsect->LockIndexBuffer ( D3DLOCK_READONLY, ( void** ) &pIndexData32 );

            dwIndex[0] = pIndexData32[m_InsectInfo.dwFaceIndex * 3 + 0];
            dwIndex[1] = pIndexData32[m_InsectInfo.dwFaceIndex * 3 + 1];
            dwIndex[2] = pIndexData32[m_InsectInfo.dwFaceIndex * 3 + 2];
        }


        mMeshForInsect->UnlockIndexBuffer();

        D3DXVECTOR3 *pVertexData;

        hr = mMeshForInsect->LockVertexBuffer ( D3DLOCK_READONLY, ( void** ) &pVertexData );

        v[0] = pVertexData[dwIndex[0]];
        v[1] = pVertexData[dwIndex[1]];
        v[2] = pVertexData[dwIndex[2]];

        mMeshForInsect->UnlockVertexBuffer();

        D3DXVECTOR4 vNormal ( ZEROFLOAT, ZEROFLOAT, ZEROFLOAT, ZEROFLOAT );
        D3DXVECTOR4 vHitPos ( ZEROFLOAT, ZEROFLOAT, ZEROFLOAT, ZEROFLOAT );

        D3DXVECTOR3 vTmp1, vTmp2;
        vTmp1 = v[1] - v[0];
        vTmp2 = v[2] - v[0];

        vHitPos = ( D3DXVECTOR4 ) v[0] + m_InsectInfo.u * ( D3DXVECTOR4 ) vTmp1 + m_InsectInfo.v * ( D3DXVECTOR4 ) vTmp2;
        vHitPos.w = 1;

        updateWorld ();

        D3DXVec4Transform ( &vHitPos, &vHitPos, &getTrans()->getLocalD3D() );
        m_InsectInfo.vHitPos = D3DXVECTOR3 ( vHitPos.x, vHitPos.y, vHitPos.z );

        D3DXVec3Cross ( ( D3DXVECTOR3* ) &vNormal, &vTmp1, &vTmp2 );
        vNormal.w = 0;


        D3DXVec4Transform ( &vNormal, &vNormal, &matWorld );
        D3DXVec3Normalize ( ( D3DXVECTOR3* ) &vNormal, ( D3DXVECTOR3* ) &vNormal );

        m_InsectInfo.vNormal = D3DXVECTOR3 ( vNormal.x, vNormal.y, vNormal.z );

    }

    return mNodeState[eObjState_Picked];

}

bool GStillEntity::pick ( const POINT& pt )
{

    D3DXMATRIX	matView, matProj;

    D9DEVICE->GetDvc()->GetTransform ( D3DTS_VIEW, &matView );
    D9DEVICE->GetDvc()->GetTransform ( D3DTS_PROJECTION, &matProj );

    ////射线的起点为眼睛（在视图空间中为坐标原点（0,0,0））
    D3DXVECTOR4 vOrigin ( 0, 0, 0, 1 );

    ////射线方向为眼睛看向鼠标的朝向
    D3DXVECTOR4 vDir;

    //将鼠标位置从2D平面转换到3D的视图空间中
    vDir.x = ( ( ( 2.0f * pt.x ) / D9DEVICE->mWidth ) - 1 ) / matProj._11;
    vDir.y = - ( ( ( 2.0f * pt.y ) / D9DEVICE->mHeight ) - 1 ) / matProj._22;
    vDir.z =  1.0f;
    vDir.w =  0.0f;

    D3DXMatrixInverse ( &matView, NULL, &matView );

    //将vDir和vPos转换到世界坐标系中
    D3DXVec4Transform ( &vDir, &vDir, &matView );
    D3DXVec4Normalize ( &vDir, &vDir );

    D3DXVec4Transform ( &vOrigin, &vOrigin, &matView );

    return checkIntersect ( vOrigin, vDir, false );

}



void GStillEntity::registerAllProperty()
{
    __super::registerAllProperty();
}





