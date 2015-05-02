#pragma once
struct D3DXMeshContainerEX;
class GAllocateHierarchy: public ID3DXAllocateHierarchy	//扩展D3D分配层次类
{
public:

	GAllocateHierarchy( IDirect3DDevice9 *DVC=nullptr,const char* mediaFile=nullptr);

	~GAllocateHierarchy();

public:

	STDMETHOD( CreateFrame )( LPCSTR Name, LPD3DXFRAME *ppNewFrame );

	STDMETHOD( CreateMeshContainer )(
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer ) ;

	STDMETHOD( DestroyFrame )( LPD3DXFRAME pFrameToFree ) ;

	STDMETHOD( DestroyMeshContainer )( LPD3DXMESHCONTAINER pMeshContainerToFree ) ;

private:

	HRESULT GenerateSkinnedMesh( D3DXMeshContainerEX *pMeshContainerEx );

	uString mMediaFileName;

};
