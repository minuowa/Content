
#pragma once
#include "XArea.h"
#include "GRenderObject.h"

struct Particle							//粒子的数据结构
{
	DWORD Color;						//粒子颜色

	D3DXVECTOR3 Translate;				//粒子位置

	D3DXVECTOR3 Speed;					//粒子速度

	D3DXVECTOR3 Rotate;					//当前的旋转位置

	float SpeedRotate;					//旋转速度

	float LifeTime;						//粒子剩余生命

	float TotalTime;					//粒子生命周期

	bool bOwnDimension;					//是否可控制粒子大小

};

#define D3DFVF_SNOW D3DFVF_XYZ | D3DFVF_TEX1

struct VertexSnow
{
	float x, y, z;

	float u, v;
};

class CRectMesh: public GRenderObject
{

public:

	CRectMesh( void );

	~CRectMesh( void );

public:

	bool recreate();

	void update();

	virtual bool render();

public:

	int LnNumParticles;						//粒子数量

	IDirect3DVertexBuffer9 *mpVB;

	IDirect3DTexture9 *mpFace;

	Particle *pSnow;						//雪花粒子数组

	CAreaSphere	mAreaSphere;

};

extern CRectMesh gSnow;