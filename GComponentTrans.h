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
struct GAutoMoveInfo 
{
	CXMillSecond mAutoLifeTime;
	/** @brief auto move/trun time point **/
	CXMillSecond mAutoInitTime;			
	D3DXQUATERNION mAutoTargetRotation;
	D3DXQUATERNION mAutoInitRotation;
	D3DXVECTOR3 mAutoTargetTranslate;
	D3DXVECTOR3 mAutoInitTranslate;
	GAutoMoveInfo();
	DWORD getElpaseTime();
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
	virtual void set()override;
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

    void update();
	void updateTranslate();
	void updateRotation();
	void moveTo(D3DXMATRIX& target,DWORD millSeconds);
private:
	void setRotation(D3DXMATRIX& mat);

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
    D3DXVECTOR3 mZoom;

    //物体相对于自身原点的旋转角度
    D3DXVECTOR3 mBodyPass;
    D3DXVECTOR3 mBodyRote;

    //速度
    D3DXVECTOR3 mSpeed;
    float mSpeedMove;
    float mSpeedTrun;

    D3DXVECTOR3 mTargetPos;

    D3DXVECTOR3 mTargetDir;

    bool mAutoMove;

    bool mAutoTrun;

    bool mJump;

    bool mCanMoveStep;					//是否可以步进移动

    bool mBack;						//是否向后行走

	GAutoMoveInfo* mAutoMoveInfo;
};
