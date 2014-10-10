#pragma once
#include "XSingleton.h"
#include "GResourceManager.h"
struct D3DXFrameEX : public D3DXFRAME	//扩展的D3D框架
{
	D3DXMATRIX matCombinedTransformation;
	//组合变换矩阵；

	//(遍历父框架的Transformation:T1、T2、...，得到组合矩阵ParentMat=T1*T2*...，matCombined=matWorld*ParantMat*Transform)
};

struct D3DXMeshContainerEX: public D3DXMESHCONTAINER		//扩展的D3D网格容器
{
	/******************************************************************/
	//在CreateMeshContainer中取值的成员
	//Name;MeshData;pMaterials;NumMaterials;pSkinInfo

	ID3DXMesh *pOriginMesh;				//骨骼模型原有Mesh

	IDirect3DTexture9 **ppTexture;		//纹理组

	D3DXMATRIX *pBoneOffsetMatrices;	//初始偏移矩阵(相对于本地空间的)

	/******************************************************************/

	D3DXMATRIX**  ppBoneMatrixPtrs;		//在SetupBoneMatixPointer过程中设置，*ppBoneMatrixPtrs指向pFrameEx->matCombinedTransformation

	/******************************************************************/
	//在GenerateSkinnedMesh中取值的成员
	ID3DXBuffer *pBoneCombinationBufffer;//指向D3DXBONECOMBINATION数组的指针，可以获取BoneID

	DWORD NumInfl;						//影响每个需要实施蒙皮的顶点的骨骼最大数量

	DWORD NumAttributeGroups;			//骨骼混合表中SubMesh的数量

	~D3DXMeshContainerEX();
};
struct GBoneLinker			//悬挂物品用的骨骼信息
{
	GString	mName;
	D3DXMATRIX* mTransform;
	GBoneLinker()
		: mTransform ( 0 )
	{
	}
};
typedef CXMap<GString, GBoneLinker*> GBoneLinkerMap;
class GAnimationResource
{
public:
	GAnimationResource()
		: mFrameRoot ( 0 )
		, mAnimationController ( 0 )
	{
	}
	~GAnimationResource();
public:
	bool createFromFile ( const char* name );
	void UpdateBones();
	GBoneLinker *GetBoneInfo( CChar*sBoneName );
protected:
	HRESULT SetupBoneMatrixPointers( LPD3DXFRAME pFrame )  ;
	HRESULT SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER pMeshContainerBase );
	int AddBoneInfo( CChar*sBoneName, D3DXMATRIX *pmat );
public:
	D3DXFRAME*	mFrameRoot;									//根框架
	GBoneLinkerMap mBoneInfos;
	ID3DXAnimationController* mAnimationController;			//动画控制器
};
typedef CXSingleton<GResourceManager<GAnimationResource>> GAnimationManager;



