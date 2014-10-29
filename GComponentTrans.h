#pragma once
#include "GComponentInterface.h"
#include "GFactory.h"


//碰撞信息
struct IntersectInfo
{
    DWORD dwFaceIndex;
    float u, v;
    float fDist;
    D3DXVECTOR3 vNormal;		//已经转化过的向量
    D3DXVECTOR3 vHitPos;		//已经转化过的坐标（世界空间的坐标）
    void *pObj;					//碰撞对象的指针

    IntersectInfo();

};

class GComponentTrans: public GComponentBase<eComponentType_Trans>
{
    DeclareComponentType ( GComponentTrans )
public:
    GComponentTrans ( void );
    ~GComponentTrans ( void );
public:
    virtual void registerAllProperty();
public:
    D3DXMATRIX GetWorldMatrix ( bool bForTrans = false );

    D3DXMATRIX GetRotationMatrix ( bool bForTrans = false );

    D3DXMATRIX GetScaleMatrix ( bool bForTrans = false );

    D3DXMATRIX GetTransLation ( bool bForTrans = false );

    D3DXVECTOR3 MoveStep ( float fPass );
    D3DXVECTOR3 TrunStepLeftRight ( float fPara );
    D3DXVECTOR3 TrunStepLeftRightWithUp ( float fpara );
    D3DXVECTOR3 TrunWithRight ( float fPara );

    int Jump();

    D3DXVECTOR3 MoveToPos ( D3DXVECTOR3 vTarget );

    D3DXVECTOR3 TrunToDir ( D3DXVECTOR3 vTargetDir );

    D3DXVECTOR3 SetDirWithUpon ( D3DXVECTOR3 vUpon );

    void Update();

private:

    D3DXVECTOR3 AutoMoveTo ( float fPass );

    virtual const char* GetComponentName();

public:

    D3DXVECTOR3 mvLastPos;
    //位置
    D3DXVECTOR3 mTranslate;

    //方向，右方向，上方向
    D3DXVECTOR3 mDir;
    D3DXVECTOR3 mRight;
    D3DXVECTOR3 mUpon;

    //缩放系数
    D3DXVECTOR3 mvZoom;

    //物体相对于自身原点的旋转角度
    D3DXVECTOR3 mvBodyPass;
    D3DXVECTOR3 mBodyRote;

    //速度
    D3DXVECTOR3 mvSpeed;
    float mfSpeedMove;
    float mfSpeedTrun;

    D3DXVECTOR3 mvTargetPos;

    D3DXVECTOR3 mvTargetDir;

    bool mbAutoMoveWithTarget;

    bool mbAutoTrunWithTarget;

    bool mbAutoMove;

    bool mbJump;

    //IntersectInfo mInsectMapInfo;



    bool mbCanMoveStep;					//是否可以步进移动

    //bool mbIsEye;						//是否是眼睛，眼睛不会受强制逻辑

    bool mbBack;						//是否向后行走
};
