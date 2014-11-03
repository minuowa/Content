#pragma once



#include "GCuller.h"
#include "GD8Input.h"

#include "GNode.h"

#define LEN_TRACE_MAN 300	//眼睛与被跟踪物的水平距离

#define HEIGHT_TRACE_MAN 300	//眼睛与被跟踪物的垂直距离
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

    virtual void update();
    virtual void onCallBack ( const CXDelegate& delgate );
    void cliper ( GNode* n );
	float getNear()const;
	float getFar()const;
	float getFov() const;
	float getAspect() const;
	void getObjectCorrdPos(D3DXVECTOR3& out,GNode* obj);
	GFrustum* getCuller() const;
public:

    void TraceMan();

    bool InitTrack ( GNode *pWorldObj );

    float mfLenTransMan;

    bool mbTraceMan;



public:

    D3DXMATRIX matProj;

    D3DXMATRIX matView;

    GFrustum *mCuller;

private:

    GNode *mpTransMan;			//摄像机跟踪的人物

    float mMouseZoomFactor;
    float mNear;
    float mFieldOfView;
    float mFar;
	float mAspect;

};


//extern CEye gEye;//全局观察对象