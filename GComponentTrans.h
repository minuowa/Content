#pragma once
#include "GComponentInterface.h"
#include "GFactory.h"
#include "GMatrix.h"


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
    DeclareFilmObj ( GComponentTrans );
public:
    GComponentTrans ( void );
    ~GComponentTrans ( void );
public:
    virtual void registerAllProperty();
public:
    virtual void set() override;
    void getLocal ( D3DXMATRIX& out ) const;
    void updateWorld ( const GMatrix& parentWorldMat );

    const GMatrix& getLocal() const;
    const D3DXMATRIX& getLocalD3D() const;

    const GMatrix& getWorld() const;
    const D3DXMATRIX& getWorldD3D() const;

    const D3DXVECTOR3& getDir() const;
    const D3DXVECTOR3& getUpon() const;
    const D3DXVECTOR3& getRight() const;
    const D3DXVECTOR3& getTranslate() const;

    const D3DXVECTOR3& getWorldDir() const;
    const D3DXVECTOR3& getWorldUpon() const;
    const D3DXVECTOR3& getWorldRight() const;
    const D3DXVECTOR3& getWorldTranslate() const;


    void setDir ( const D3DXVECTOR3& v );
    void setUpon ( const D3DXVECTOR3& v );
    void setRight ( const D3DXVECTOR3& v );
    void setTranslateV ( const D3DXVECTOR3& v );

    DeclareFilmTool void setTranslate ( float x, float y, float z );

    void setWorldDir ( const D3DXVECTOR3& v );
    void setWorldUpon ( const D3DXVECTOR3& v );
    void setWorldRight ( const D3DXVECTOR3& v );
    void setWorldTranslate ( const D3DXVECTOR3& v );

    void normalizeDir();
    void normalizeRight();
    void normalizeUpon();
    void normalizeRotation();

    D3DXVECTOR3 MoveStep ( float fPass );
    void TrunStepLeftRight ( float fPara );
    void trunWithUpon ( float fpara );
    void trunWithRight ( float fPara );

    int Jump();

    D3DXVECTOR3 MoveToPos ( D3DXVECTOR3 vTarget );

    D3DXVECTOR3 TrunToDir ( D3DXVECTOR3 vTargetDir );

    D3DXVECTOR3 SetDirWithUpon ( D3DXVECTOR3 vUpon );
    void update();
    void moveTo ( const GMatrix& target, DWORD millSeconds );
    void setYawPitchRow ( float yDegree, float pDegree, float rDegree );
	void noitifyWorldNeedUpdate();
private:
    void setRotation ( D3DXMATRIX& mat );
    void updateDirUponRight2YawPitchRoll();
    void updateYawPitchRowRotation();
    void updateAutoTranslate();
    void updateAutoMoveRotation();
    virtual const char* getComponentName();

    virtual void onPropertyChangeEnd ( void* cur );

protected:
    GMatrix mMatLocal;
    GMatrix mMatWorld;
    float mDegreeYaw;
    float mDegreePitch;
    float mDegreeRoll;
public:

    D3DXVECTOR3 mvLastPos;


    //缩放系数
    D3DXVECTOR3 mZoom;

    //物体相对于自身原点的旋转角度
    D3DXVECTOR3 mBodyPass;
    D3DXVECTOR3 mBodyRote;

    //速度
    D3DXVECTOR3 mSpeed;
    float mSpeedTrun;
    float mSpeedMove;

    D3DXVECTOR3 mTargetPos;

    D3DXVECTOR3 mTargetDir;

    bool mAutoMove;

    bool mAutoTrun;

    bool mJump;

    bool mCanMoveStep;					//是否可以步进移动

    bool mBack;						//是否向后行走

    bool mNeedUpdate;
	bool mNeedUpdateYawPitchRow;

    GAutoMoveInfo* mAutoMoveInfo;
};
inline const GMatrix& GComponentTrans::getLocal() const
{
    return mMatLocal;
}
inline void GComponentTrans::normalizeDir()
{
    dVector3Normalize ( mMatLocal.mDir );
}
inline const GMatrix& GComponentTrans::getWorld() const
{
    return mMatWorld;
}

inline const D3DXMATRIX& GComponentTrans::getWorldD3D() const
{
    return  * ( ( const D3DXMATRIX* ) ( &mMatLocal ) );
}
inline void GComponentTrans::normalizeRight()
{
    dVector3Normalize ( mMatLocal.mRight );
}

inline void GComponentTrans::normalizeUpon()
{
    dVector3Normalize ( mMatLocal.mUpon );
}
inline void GComponentTrans::normalizeRotation()
{
    normalizeDir();
    normalizeUpon();
    normalizeRight();
	updateDirUponRight2YawPitchRoll();
}

inline const D3DXVECTOR3& GComponentTrans::getDir() const
{
    return mMatLocal.mDir;
}

inline const D3DXVECTOR3& GComponentTrans::getUpon() const
{
    return mMatLocal.mUpon;
}

inline const D3DXVECTOR3& GComponentTrans::getRight() const
{
    return mMatLocal.mRight;
}
inline const D3DXVECTOR3& GComponentTrans::getWorldDir() const
{
    return mMatWorld.mDir;
}

inline const D3DXVECTOR3& GComponentTrans::getWorldUpon() const
{
    return mMatWorld.mUpon;
}

inline const D3DXVECTOR3& GComponentTrans::getWorldRight() const
{
    return mMatWorld.mRight;
}
inline void GComponentTrans::setDir ( const D3DXVECTOR3& v )
{
    mMatLocal.mDir = v;
    normalizeDir();
    mNeedUpdate = true;
	mNeedUpdateYawPitchRow=true;
}

inline void GComponentTrans::setUpon ( const D3DXVECTOR3& v )
{
    mMatLocal.mUpon = v;
    normalizeUpon();
    mNeedUpdate = true;
	mNeedUpdateYawPitchRow=true;
}

inline void GComponentTrans::setRight ( const D3DXVECTOR3& v )
{
    mMatLocal.mRight = v;
    normalizeRight();
    mNeedUpdate = true;
	mNeedUpdateYawPitchRow=true;
}
inline void GComponentTrans::setWorldDir ( const D3DXVECTOR3& v )
{
    mMatWorld.mDir = v;
    mNeedUpdate = true;
}

inline void GComponentTrans::setWorldUpon ( const D3DXVECTOR3& v )
{
    mMatWorld.mUpon = v;
    mNeedUpdate = true;
}

inline void GComponentTrans::setWorldRight ( const D3DXVECTOR3& v )
{
    mMatWorld.mRight = v;
    mNeedUpdate = true;
}
inline const D3DXMATRIX& GComponentTrans::getLocalD3D() const
{
    return  * ( ( const D3DXMATRIX* ) ( &mMatLocal ) );
}

inline const D3DXVECTOR3& GComponentTrans::getTranslate() const
{
    return mMatLocal.mTranslate;
}
inline const D3DXVECTOR3& GComponentTrans::getWorldTranslate() const
{
    return mMatWorld.mTranslate;
}
inline void GComponentTrans::setTranslateV ( const D3DXVECTOR3& v )
{
    setTranslate ( v.x, v.y, v.z );
}