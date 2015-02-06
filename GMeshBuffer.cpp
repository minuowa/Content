#include "GGameDemoHeader.h"
#include "GMeshBuffer.h"

#include "GTexture.h"
#include "GStillEntity.h"
#include "GSceneMgr.h"
#include "GD9Device.h"
//////////////////////////////////////////////////////////////////////////
GMeshManager::GMeshManager ( void )
{
}

GMeshManager::~GMeshManager ( void )
{
}

GMeshBufferNode* GMeshManager::QueryCreate ( const char* fileName )
{
	GMeshBufferNode* node = 0;
	if ( !mMeshMap.Get ( fileName, node ) )
	{
		node = CreateFormFile ( fileName );
		CXASSERT_RETURN_FALSE ( node );
		mMeshMap.Insert ( fileName, node );
	}
	return node;
}


bool GMeshManager::Init()
{
	return true;
}

//void *GMeshBuffer::GetInput( float fPass )
//{
//
//    void *pSelectObj = NULL;
//
//    POINT pt = INPUTSYSTEM.GetMousePoint();
//
//    bool bHit = false;
//
//    if ( INPUTSYSTEM.GetButtonAction( btLB ) == DI_BUTTONUP )
//    {
//        for ( CMeshBufferNodeArr::iterator i = mMeshList.begin(); i != mMeshList.end(); ++i )
//        {
//            GMeshBufferNode* node = *i;
//            CXMeshBaseObjList& list = node->GetObjList();
//
//            for ( CXMeshBaseObjList::iterator iter = list.begin(); iter != list.end(); ++iter )
//            {
//                CXMeshBaseObj *pObj = *iter;
//
//                if ( !bHit )
//                {
//                    if ( pObj->m_bCanSelect )
//                    {
//                        bHit = pObj->Pick( pt );
//
//                        if ( bHit )
//                        {
//                            pSelectObj = pObj;
//                            //pObj->mbChangeWhenHit=true;
//                            pObj->m_bBeSelected = true;
//                            break;
//                        }
//                        else
//                        {
//                            pObj->m_bBeSelected = false;
//                            //pObj->mbChangeWhenHit=false;
//                        }
//                    }
//                }
//            }
//        }
//
//        return pSelectObj;
//
//    }
//	return pSelectObj;
//}
GMeshBufferNode* GMeshManager::CreateFormFile ( const char* fileName )
{
	HRESULT hr = S_FALSE;

	LPD3DXBUFFER pAdj = NULL;
	LPD3DXBUFFER pMat = NULL;
	DWORD LnAttrNum = 0;

	ID3DXMesh* rootMesh = 0;

	hr = D3DXLoadMeshFromXA ( fileName,
		D3DXMESH_MANAGED | D3DXMESH_32BIT,
		D9Device->GetDvc(), &pAdj, &pMat, NULL, &LnAttrNum, &rootMesh );

	CXASSERT_RESULT_FALSE ( hr );

	GMeshBufferNode* node = new GMeshBufferNode;
	node->mFileName = fileName;
	node->mMesh = rootMesh;
	node->setSubCount ( LnAttrNum );

	D3DXMATERIAL *pMatList = ( D3DXMATERIAL * ) pMat->GetBufferPointer();

	for ( DWORD i = 0; i < LnAttrNum; i++ )
	{
		GMetrialData* metrialData = new GMetrialData;

		metrialData->setMetiral ( pMatList[i].MatD3D );
		CXFileName path ( fileName );
		GString textureName = path.GetRelativePath();

		if ( pMatList[i].pTextureFilename != NULL )
		{
			textureName.append ( pMatList[i].pTextureFilename );
			metrialData->setTexture ( textureName );
		}
	}
	return node;
}

GMeshBufferNode::GMeshBufferNode()
{

}



bool GMeshBufferNode::draw()
{
	CXASSERT_RESULT_FALSE ( mRenderData.size() == mSubSetCount );
	CXASSERT_RETURN_FALSE ( mMesh );

	for ( int i = 0; i < mSubSetCount; ++i )
	{
		GMetrialData* renderData = mRenderData[i];
		renderData->set();
		mMesh->DrawSubset ( i );
	}
	return true;
}

HRESULT GMeshBufferNode::MakeLod ( DWORD* pAdj )
{
	return S_OK;
}

GMeshBufferNode::~GMeshBufferNode()
{
	dSafeRelease ( mMesh );
	dSafeDeleteVector ( mRenderData );
}

GMetrialData* GMeshBufferNode::getMaterial( CXIndex idx ) const
{
	return mRenderData[idx];
}
