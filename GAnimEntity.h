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
    LPD3DXANIMATIONSET mpAnimSet;						//��ǰ������
    bool mbPlayDone;									//��һ���������Ƿ񲥷����
    DWORD mdwOldAnimSetFrame;							//��ǰ��������һ�β��ŵ���֡��
    DWORD mdwCurAnimSetFrame;							//��ǰ�������˴β��ŵ���֡��
private:
    GAnimationResource*	mResource;
    ID3DXAnimationController *mCloneAnimationController;			//����������
public:
};
inline GAnimationResource* GAnimEntity::GetResource() const
{
    return mResource;
}

/***************************************************************************

ÿ��Frame�����һ��MeshContainer��

ÿ��Meshcontainer��NumAttributeGroup��MeshSubSet��

ÿ��MeshSubSet��NumInfl��������Ӱ�죬

***************************************************************************/