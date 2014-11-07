
#pragma once
#include "XArea.h"
#include "GRenderObject.h"

struct Particle							//���ӵ����ݽṹ
{
	DWORD Color;						//������ɫ

	D3DXVECTOR3 Translate;				//����λ��

	D3DXVECTOR3 Speed;					//�����ٶ�

	D3DXVECTOR3 Rotate;					//��ǰ����תλ��

	float SpeedRotate;					//��ת�ٶ�

	float LifeTime;						//����ʣ������

	float TotalTime;					//������������

	bool bOwnDimension;					//�Ƿ�ɿ������Ӵ�С

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

	int LnNumParticles;						//��������

	IDirect3DVertexBuffer9 *mpVB;

	IDirect3DTexture9 *mpFace;

	Particle *pSnow;						//ѩ����������

	CAreaSphere	mAreaSphere;

};

extern CRectMesh gSnow;