#include "GGameDemoHeader.h"
#include "GAllocateHierarchy.h"
#include "GAnimationResource.h"
#include "GD9Device.h"
#include "GTexture.h"
#include "Content.h"

HRESULT AllocateName ( LPCTSTR Name, LPTSTR *pNewName )
{
	//Name为空值，返回错误
	if ( Name == NULL )
	{
		*pNewName = NULL;


		return S_FALSE;
	}

	int nLen = lstrlen ( Name ) + 1;

	*pNewName = new TCHAR[nLen];

	//内存不足，返回错误
	if ( *pNewName == NULL )
	{
		return E_OUTOFMEMORY;
	}

	memcpy ( *pNewName, Name, nLen * sizeof ( TCHAR ) );

	return S_OK;

}
STDMETHODIMP GAllocateHierarchy::CreateFrame ( LPCSTR Name, LPD3DXFRAME *ppNewFrame )
{

	HRESULT hr = S_FALSE;

	*ppNewFrame = NULL;

	D3DXFrameEX *pFrameTmp;
	pFrameTmp = new D3DXFrameEX;

	if ( pFrameTmp == NULL )
	{
		delete ( pFrameTmp );
		return E_OUTOFMEMORY;
	}

	hr = AllocateName ( ( LPCTSTR ) Name, ( LPTSTR * ) ( &pFrameTmp->Name ) );

	if ( FAILED ( hr ) )
	{
		delete ( pFrameTmp );
		return S_FALSE;
	}

	D3DXMatrixIdentity ( &pFrameTmp->TransformationMatrix );

	D3DXMatrixIdentity ( &pFrameTmp->matCombinedTransformation );

	pFrameTmp->pFrameFirstChild = NULL;

	pFrameTmp->pFrameSibling = NULL;

	pFrameTmp->pMeshContainer = NULL;

	*ppNewFrame = pFrameTmp;

	pFrameTmp = NULL;

	return S_OK;

}


STDMETHODIMP GAllocateHierarchy::CreateMeshContainer
	(
	LPCSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppNewMeshContainer
	)
{

	*ppNewMeshContainer = NULL;

	ID3DXMesh *pMesh = NULL;

	HRESULT hr = S_FALSE;

	D3DXMeshContainerEX *pMeshContainerTmp = NULL;	//临时MeshContainer变量，为了给ppNewMeshContainer赋予完善的信息

	IDirect3DDevice9 *pD9Device = NULL;				//在创建纹理中要用到

	DWORD dwNumFace = 0;								//pMeshContainer->MeshData.pMesh中Mesh的面数

	DWORD dwNumBone = 0;								//影响该容器中Mesh的骨骼数量


	//获取MeshData
	if ( pMeshData->Type != D3DXMESHTYPE_MESH || pMeshData->pMesh == NULL )
	{
		return S_FALSE;
	}

	if ( pMeshData->pMesh->GetFVF() == 0 )
	{
		return S_FALSE;
	}

	pMeshContainerTmp = new D3DXMeshContainerEX;

	if ( pMeshContainerTmp == NULL )
	{
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}

	ZeroMemory ( pMeshContainerTmp, sizeof ( D3DXMeshContainerEX ) );

	pMeshData->pMesh->GetDevice ( &pD9Device );

	if ( pD9Device == NULL )
	{
		goto e_Exit;
	}

	pMesh = pMeshData->pMesh;

	if ( ! ( pMesh->GetFVF() && D3DFVF_NORMAL ) )	//没有法线信息为其添加一个
	{
		hr = pMeshData->pMesh->CloneMeshFVF (
			pMeshData->pMesh->GetOptions(),
			D3DFVF_NORMAL | pMeshData->pMesh->GetFVF(),
			pD9Device,
			&pMeshContainerTmp->MeshData.pMesh
			);

		if ( FAILED ( hr ) )
		{
			goto e_Exit;
		}

		hr = D3DXComputeNormals ( pMeshContainerTmp->MeshData.pMesh, 0 );

		if ( FAILED ( hr ) )
		{
			goto e_Exit;
		}

		pMesh = pMeshContainerTmp->MeshData.pMesh;
	}
	else
	{
		pMeshContainerTmp->MeshData.pMesh = pMesh;

		pMesh->AddRef();
	}

	pMeshContainerTmp->MeshData.Type = D3DXMESHTYPE_MESH;

	//分配容器名字
	hr = AllocateName ( ( LPCTSTR ) Name, ( LPTSTR * ) ( &pMeshContainerTmp->Name ) );

	if ( FAILED ( hr ) )
	{
		goto e_Exit;
	}

	//获取材质信息

	dwNumFace = pMeshContainerTmp->MeshData.pMesh->GetNumFaces();

	pMeshContainerTmp->NumMaterials = max ( 1, NumMaterials );

	pMeshContainerTmp->pAdjacency = new DWORD[dwNumFace * 3];

	pMeshContainerTmp->ppTexture = new IDirect3DTexture9*[pMeshContainerTmp->NumMaterials];

	pMeshContainerTmp->pMaterials = new D3DXMATERIAL[pMeshContainerTmp->NumMaterials];

	if ( pMeshContainerTmp->ppTexture == NULL || pMeshContainerTmp->pMaterials == NULL )
	{
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}

	memcpy ( pMeshContainerTmp->pAdjacency, pAdjacency, sizeof ( DWORD ) *dwNumFace * 3 );
	ZeroMemory ( pMeshContainerTmp->ppTexture, pMeshContainerTmp->NumMaterials * sizeof ( IDirect3DTexture9* ) );

	if ( NumMaterials > 0 )
	{
		memcpy ( pMeshContainerTmp->pMaterials, pMaterials, sizeof ( D3DXMATERIAL ) *NumMaterials );

		CXFileName filename ( mMediaFileName );
		GString path = filename.GetRelativePath();

		for ( DWORD i = 0; i < NumMaterials; i++ )
		{
			if ( pMeshContainerTmp->pMaterials[i].pTextureFilename != NULL )
			{
				GString fileName = path + CXFileName::PathSpliter + pMeshContainerTmp->pMaterials[i].pTextureFilename;
				GTexture* gtext =  Content::TextureMgr.getResource ( fileName.c_str() );
				if ( gtext )
					pMeshContainerTmp->ppTexture[i] = gtext->getTexture();
			}
		}
	}
	else	//没有材质信息就为其添加一个
	{
		pMeshContainerTmp->pMaterials[0].pTextureFilename = NULL;
		pMeshContainerTmp->pMaterials[0].MatD3D.Diffuse.r = 0.8f;
		pMeshContainerTmp->pMaterials[0].MatD3D.Diffuse.g = 0.8f;
		pMeshContainerTmp->pMaterials[0].MatD3D.Diffuse.b = 0.8f;

		pMeshContainerTmp->pMaterials[0].MatD3D.Specular = pMeshContainerTmp->pMaterials[0].MatD3D.Diffuse;
	}

	if ( pSkinInfo != NULL )
	{
		pMeshContainerTmp->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		pMeshContainerTmp->pOriginMesh = pMesh;
		pMesh->AddRef();

		dwNumBone = pMeshContainerTmp->pSkinInfo->GetNumBones();

		pMeshContainerTmp->pBoneOffsetMatrices = new D3DXMATRIX[dwNumBone];

		if ( pMeshContainerTmp->pBoneOffsetMatrices == NULL )
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

		for ( DWORD i = 0; i < dwNumBone; i++ )
		{
			pMeshContainerTmp->pBoneOffsetMatrices[i] = * ( pMeshContainerTmp->pSkinInfo->GetBoneOffsetMatrix ( i ) );
		}

	}

	hr = GenerateSkinnedMesh ( pMeshContainerTmp );

	*ppNewMeshContainer = pMeshContainerTmp;
	pMeshContainerTmp = NULL;

e_Exit:

	SAFERELEASE ( pD9Device );

	if ( pMeshContainerTmp != NULL )
	{
		DestroyMeshContainer ( pMeshContainerTmp );
	}

	return S_OK;
}

GAllocateHierarchy::GAllocateHierarchy ( IDirect3DDevice9 * DVC, const char* mediaFile )
{
	if ( mediaFile != nullptr )
		mMediaFileName = mediaFile;
}

HRESULT GAllocateHierarchy::DestroyMeshContainer ( LPD3DXMESHCONTAINER pMeshContainerToFree )
{
	D3DXMeshContainerEX *pMeshContainerTmp = ( D3DXMeshContainerEX * ) pMeshContainerToFree;

	SAFE_DELETE_ARRAY ( pMeshContainerTmp->Name );
	SAFE_DELETE_ARRAY ( pMeshContainerTmp->pAdjacency );
	SAFE_DELETE_ARRAY ( pMeshContainerTmp->pBoneOffsetMatrices );
	SAFE_DELETE_ARRAY ( pMeshContainerTmp->pMaterials );

	if ( pMeshContainerTmp->ppTexture != NULL )
	{
		for ( DWORD i = 0; i < pMeshContainerTmp->NumMaterials; i++ )
		{
			SAFERELEASE ( pMeshContainerTmp->ppTexture[i] );
		}
	}

	SAFE_DELETE_ARRAY ( pMeshContainerTmp->ppTexture );
	SAFE_DELETE_ARRAY ( pMeshContainerTmp->ppBoneMatrixPtrs );

	SAFERELEASE ( pMeshContainerTmp->pOriginMesh );
	SAFERELEASE ( pMeshContainerTmp->MeshData.pMesh );
	SAFERELEASE ( pMeshContainerTmp->pSkinInfo );
	SAFERELEASE ( pMeshContainerTmp->pBoneCombinationBufffer );

	SAFED_ELETE ( pMeshContainerTmp );

	return S_OK;
}

HRESULT GAllocateHierarchy::GenerateSkinnedMesh ( D3DXMeshContainerEX *pMeshContainerEx )
{

	HRESULT hr = S_FALSE;

	D3DCAPS9 d3dCap;
	ZeroMemory ( &d3dCap, sizeof ( d3dCap ) );

	 Content::Device.GetDvc()->GetDeviceCaps ( &d3dCap );

	if ( pMeshContainerEx->pSkinInfo == NULL )
	{
		return S_FALSE;
	}

	SAFERELEASE ( pMeshContainerEx->MeshData.pMesh );
	SAFERELEASE ( pMeshContainerEx->pBoneCombinationBufffer );

	hr = pMeshContainerEx->pSkinInfo->ConvertToBlendedMesh
		(
		pMeshContainerEx->pOriginMesh,
		D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE,
		pMeshContainerEx->pAdjacency,
		NULL, NULL, NULL,
		&pMeshContainerEx->NumInfl,
		&pMeshContainerEx->NumAttributeGroups,
		&pMeshContainerEx->pBoneCombinationBufffer,
		&pMeshContainerEx->MeshData.pMesh
		);

	if ( FAILED ( hr ) )
	{
		return hr;
	}

	D3DXBONECOMBINATION *pBoneCombinations = ( D3DXBONECOMBINATION* ) ( pMeshContainerEx->pBoneCombinationBufffer->GetBufferPointer() );

	DWORD dwIndex;

	for ( dwIndex = 0; dwIndex < pMeshContainerEx->NumAttributeGroups; dwIndex++ )
	{
		DWORD dwInfl = 0;

		for ( DWORD i = 0; i < pMeshContainerEx->NumInfl; i++ )
		{
			if ( pBoneCombinations[dwIndex].BoneId[i] != UINT_MAX )
			{
				dwInfl++;
			}
		}


		if ( dwInfl > d3dCap.MaxVertexBlendMatrices )	//执行顶点混合时，设备可以支持的矩阵复合的最大数量
		{
			break;
		}
	}

	if ( dwIndex < pMeshContainerEx->NumAttributeGroups )
	{
		ID3DXMesh *pMeshTmp = NULL;

		hr = pMeshContainerEx->MeshData.pMesh->CloneMeshFVF
			(
			pMeshContainerEx->MeshData.pMesh->GetOptions() | D3DXMESH_SOFTWAREPROCESSING,
			pMeshContainerEx->MeshData.pMesh->GetFVF(),
			 Content::Device.GetDvc(), &pMeshTmp
			);

		if ( FAILED ( hr ) )
		{
			return hr;
		}

		pMeshContainerEx->MeshData.pMesh->Release();
		pMeshContainerEx->MeshData.pMesh = pMeshTmp;

		pMeshTmp = NULL;

	}

	return S_OK;

}

GAllocateHierarchy::~GAllocateHierarchy()
{

}

STDMETHODIMP GAllocateHierarchy::DestroyFrame ( LPD3DXFRAME pFrameToFree )
{
	dSafeDeleteArray ( pFrameToFree->Name );
	dSafeDelete ( pFrameToFree );
	return S_OK;
}
