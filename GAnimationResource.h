#pragma once
#include "XSingleton.h"
#include "GResourceManager.h"
struct D3DXFrameEX : public D3DXFRAME	//��չ��D3D���
{
	D3DXMATRIX matCombinedTransformation;
	//��ϱ任����

	//(��������ܵ�Transformation:T1��T2��...���õ���Ͼ���ParentMat=T1*T2*...��matCombined=matWorld*ParantMat*Transform)
};

struct D3DXMeshContainerEX: public D3DXMESHCONTAINER		//��չ��D3D��������
{
	/******************************************************************/
	//��CreateMeshContainer��ȡֵ�ĳ�Ա
	//Name;MeshData;pMaterials;NumMaterials;pSkinInfo

	ID3DXMesh *pOriginMesh;				//����ģ��ԭ��Mesh

	IDirect3DTexture9 **ppTexture;		//������

	D3DXMATRIX *pBoneOffsetMatrices;	//��ʼƫ�ƾ���(����ڱ��ؿռ��)

	/******************************************************************/

	D3DXMATRIX**  ppBoneMatrixPtrs;		//��SetupBoneMatixPointer���������ã�*ppBoneMatrixPtrsָ��pFrameEx->matCombinedTransformation

	/******************************************************************/
	//��GenerateSkinnedMesh��ȡֵ�ĳ�Ա
	ID3DXBuffer *pBoneCombinationBufffer;//ָ��D3DXBONECOMBINATION�����ָ�룬���Ի�ȡBoneID

	DWORD NumInfl;						//Ӱ��ÿ����Ҫʵʩ��Ƥ�Ķ���Ĺ����������

	DWORD NumAttributeGroups;			//������ϱ���SubMesh������

	~D3DXMeshContainerEX();
};
struct GBoneLinker			//������Ʒ�õĹ�����Ϣ
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
	D3DXFRAME*	mFrameRoot;									//�����
	GBoneLinkerMap mBoneInfos;
	ID3DXAnimationController* mAnimationController;			//����������
};
typedef CXSingleton<GResourceManager<GAnimationResource>> GAnimationManager;



