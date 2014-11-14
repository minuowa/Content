#pragma once
#include "GAmmo.h"
#include "XString.h"
#include "GRenderEntity.h"

class GAnimationResource;

class GAnimEntity: public GRenderEntity
{
    DeclareEditorType ( GAnimEntity );
public:
    GAnimEntity ( void );
    ~GAnimEntity ( void );
public:
    void updateWorldInfo();
    void setMediaFile ( const char* file );

    void play ( const char* animSetName );
    void changeTo ( const char* animSetName );

    void attachOnBone ( const char* boneName, GNode* entity );
    void detachOnBone ( const char* boneName, GNode* entity );
    GNode* getEntityOnBone ( const char* boneName, const char* entityName );

    inline GAnimationResource* GetResource() const;
public:
    virtual bool recreate() override;
    virtual void update();
protected:
    virtual bool render();
protected:
    bool Pick ( ID3DXMesh *pMesh, POINT pt );
    VOID UpdateFrameMatrices ( LPD3DXFRAME pFrameBase, const D3DXMATRIX* pParentMatrix );
    void DrawMeshContainer ( D3DXMESHCONTAINER *pMeshContainerBase, D3DXFRAME *pFrameBase );
    void DrawFrame ( D3DXFRAME *pFrameBase );
    virtual D3DXMATRIX GetWorldMatrixByBone ( char *sBoneName, bool bForTrans = false );
private:
    LPD3DXANIMATIONSET mpAnimSet;						//当前动作集
    bool mbPlayDone;									//上一个动作集是否播放完成
    DWORD mdwOldAnimSetFrame;							//当前动画集上一次播放到的帧数
    DWORD mdwCurAnimSetFrame;							//当前动画集此次播放到的帧数
private:
    GAnimationResource*	mResource;
    ID3DXAnimationController *mCloneAnimationController;			//动画控制器
public:
};
inline GAnimationResource* GAnimEntity::GetResource() const
{
    return mResource;
}

/***************************************************************************

每个Frame最多有一个MeshContainer；

每个Meshcontainer有NumAttributeGroup个MeshSubSet；

每个MeshSubSet受NumInfl个骨骼的影响，

***************************************************************************/