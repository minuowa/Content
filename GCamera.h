#pragma once



#include "EyeCliper.h"
#include "GD8Input.h"

#include "GNode.h"

#define LEN_TRACE_MAN 300	//�۾��뱻�������ˮƽ����

#define HEIGHT_TRACE_MAN 300	//�۾��뱻������Ĵ�ֱ����

class GCamera :
    public GNode
{

public:

    GCamera(void);

    ~GCamera(void);

public:

	virtual bool Create();

	virtual void GetInput(DWORD frameTimeMs); 

    void SetView();

    void SetProj();

	virtual void Update();
public:

	void TraceMan();

	bool InitTrack(GNode *pWorldObj);

	float mfLenTransMan;

	bool mbTraceMan;



public:

	D3DXMATRIX matProj;

	D3DXMATRIX matView;

	CEyeCliper *mpEyeCliper;

private:

	GNode *mpTransMan;			//��������ٵ�����

 

};


//extern CEye gEye;//ȫ�ֹ۲����