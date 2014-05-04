#pragma once
#include "GNode.h"
#include "GAmmo.h"
#include "XString.h"

struct FiLinker			//������Ʒ�õĹ�����Ϣ
{
    CharString Name;

    D3DXMATRIX* Transform;

};

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

};


class CAllocateHierarchy: public ID3DXAllocateHierarchy	//��չD3D��������
{
    public:

        CAllocateHierarchy( IDirect3DDevice9 *DVC );

        ~CAllocateHierarchy();

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

};


typedef CXMap<CharString, FiLinker*> FiLinkerMap;

class GAnimMeshObj: public GRenderObject
{
	DeclareEditorType(GAnimMeshObj);

    public:

        GAnimMeshObj( void );

        ~GAnimMeshObj( void );

    public:

		bool Create( );

        int		AfterCreate();

        HRESULT SetupBoneMatrixPointers( LPD3DXFRAME pFrame )  ;

        HRESULT SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER pMeshContainerBase );

        VOID	UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix );

        void	DrawMeshContainer( D3DXMESHCONTAINER *pMeshContainerBase, D3DXFRAME *pFrameBase );

        void	DrawFrame( D3DXFRAME *pFrameBase );

        virtual eObjAnimState SetState( eObjAnimState os, bool bBack );

        int		Render( float fPass );

        bool	Pick( ID3DXMesh *pMesh, POINT pt );

        FiLinker *GetBoneInfo( CChar*sBoneName );

        virtual D3DXMATRIX GetWorldMatrixByBone( char *sBoneName, bool bForTrans = false );

    public:

        DWORD m_nAnimMeshID;

    private:

        int AddBoneInfo( CChar*sBoneName, D3DXMATRIX *pmat );

        char m_sXFileName[FILE_NAME_LENGTH];

        char m_sTextureName[FILE_NAME_LENGTH];

    private:

        LPD3DXANIMATIONSET mpAnimSet;						//��ǰ������

        bool mbPlayDone;									//��һ���������Ƿ񲥷����

        DWORD mdwOldAnimSetFrame;							//��ǰ��������һ�β��ŵ���֡��

        DWORD mdwCurAnimSetFrame;							//��ǰ�������˴β��ŵ���֡��

        FiLinkerMap mBoneInfos;

        int LnNowBoneInfoCount;

    private:

        D3DXFRAME *mpFrameRoot;									//�����

        ID3DXAnimationController *mpAnimController;				//����������

        IDirect3DTexture9* mpFace;								//����

    public:

        CAmmoParticles *mpAmmo;										//�ڵ�����ָ��

};

/***************************************************************************

ÿ��Frame�����һ��MeshContainer��

ÿ��Meshcontainer��NumAttributeGroup��MeshSubSet��

ÿ��MeshSubSet��NumInfl��������Ӱ�죬

***************************************************************************/