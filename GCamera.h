#pragma once



#include "GCuller.h"
#include "GD8Input.h"

#include "GNode.h"

#define LEN_TRACE_MAN 300	//�۾��뱻�������ˮƽ����

#define HEIGHT_TRACE_MAN 300	//�۾��뱻������Ĵ�ֱ����
class GFrustum;
class GCamera :
    public GNode
{
    DeclareEditorType ( GCamera )
public:

    GCamera ( void );

    ~GCamera ( void );

public:
    void moveTo ( D3DXMATRIX& matrix );

    virtual bool recreate();

    virtual void getInput ( DWORD frameTimeMs );

    void setView();
    void setProj();
    inline const D3DXMATRIX* getView() const;
    inline const D3DXMATRIX* getProjection() const;
    virtual void update();
    virtual void onCallBack ( const CXDelegate& delgate );
    void cliper ( GNode* n );
    float getNear() const;
    float getFar() const;
    float getFov() const;
    float getAspect() const;
    void getObjectCorrdPos ( D3DXVECTOR3& out, GNode* obj );
    GFrustum* getCuller() const;
public:

    void TraceMan();

    bool InitTrack ( GNode *pWorldObj );

    float mfLenTransMan;

    bool mbTraceMan;
public:
    D3DXMATRIX mProj;

    D3DXMATRIX mView;

    GFrustum *mCuller;

private:

    GNode *mpTransMan;			//��������ٵ�����

    float mMouseZoomFactor;
    float mNear;
    float mFieldOfView;
    float mFar;
    float mAspect;

};
inline const D3DXMATRIX* GCamera::getView() const
{
    return &mView;
}

inline const D3DXMATRIX* GCamera::getProjection() const
{
    return &mProj;
}

//extern CEye gEye;//ȫ�ֹ۲����