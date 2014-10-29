#pragma once



#include "GCuller.h"
#include "GD8Input.h"

#include "GNode.h"

#define LEN_TRACE_MAN 300	//眼睛与被跟踪物的水平距离

#define HEIGHT_TRACE_MAN 300	//眼睛与被跟踪物的垂直距离

class GCamera :
    public GNode
{
	DeclareEditorType(GCamera)
public:

    GCamera(void);

    ~GCamera(void);

public:

	virtual bool recreate();

	virtual void getInput(DWORD frameTimeMs); 

    void setView();

    void setProj();

	virtual void update();
	virtual void onCallBack(const CXDelegate& delgate);
public:

	void TraceMan();

	bool InitTrack(GNode *pWorldObj);

	float mfLenTransMan;

	bool mbTraceMan;



public:

	D3DXMATRIX matProj;

	D3DXMATRIX matView;

	GCuller *mpEyeCliper;

private:

	GNode *mpTransMan;			//摄像机跟踪的人物

 

};


//extern CEye gEye;//全局观察对象