#pragma once



#include "GCuller.h"
#include "GD8Input.h"

#include "GNode.h"

#define LEN_TRACE_MAN 300	//�۾��뱻�������ˮƽ����

#define HEIGHT_TRACE_MAN 300	//�۾��뱻������Ĵ�ֱ����

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

	GNode *mpTransMan;			//��������ٵ�����

 

};


//extern CEye gEye;//ȫ�ֹ۲����