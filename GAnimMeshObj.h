#pragma once
#include "GAmmo.h"
#include "XString.h"
#include "GRenderObject.h"

class GAnimationResource;

class GAnimMeshObj: public GRenderObject
{
	DeclareEditorType(GAnimMeshObj);

public:
	GAnimMeshObj( void );
	~GAnimMeshObj( void );
public:
	virtual bool recreate()override;
	virtual void update();
	void updateWorldInfo();
	void setMediaFile(const char* file);

	VOID	UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix );

	void	DrawMeshContainer( D3DXMESHCONTAINER *pMeshContainerBase, D3DXFRAME *pFrameBase );

	void	DrawFrame( D3DXFRAME *pFrameBase );

	virtual eObjAnimState SetState( eObjAnimState os, bool bBack );

	virtual bool render();

	bool	Pick( ID3DXMesh *pMesh, POINT pt );


	virtual D3DXMATRIX GetWorldMatrixByBone( char *sBoneName, bool bForTrans = false );
public:
	char m_sTextureName[FILE_NAME_LENGTH];
private:
	LPD3DXANIMATIONSET mpAnimSet;						//当前动作集
	bool mbPlayDone;									//上一个动作集是否播放完成
	DWORD mdwOldAnimSetFrame;							//当前动画集上一次播放到的帧数
	DWORD mdwCurAnimSetFrame;							//当前动画集此次播放到的帧数
	int LnNowBoneInfoCount;

private:
	GAnimationResource*	mResource;
	ID3DXAnimationController *mCloneAnimationController;			//动画控制器
public:
	CAmmoParticles *mpAmmo;										//炮弹对象指针
public:
	inline GAnimationResource* GetResource() const
	{
		return mResource;
	}
};

/***************************************************************************

每个Frame最多有一个MeshContainer；

每个Meshcontainer有NumAttributeGroup个MeshSubSet；

每个MeshSubSet受NumInfl个骨骼的影响，

***************************************************************************/