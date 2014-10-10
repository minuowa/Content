#include "GGameDemoHeader.h"
#include "GCuller.h"

GCuller::GCuller(void)
{

}

GCuller::GCuller( float fn,float angle, float wh )
{
	float y=fn*tanf(angle);

	float x=y*wh;

	//顶点示意图
	//1.........2
	//4.........3
	mVertex[0]=ZEROVECTOR3;
	mVertex[1]=D3DXVECTOR3(-x,y,fn);
	mVertex[2]=D3DXVECTOR3(x,y,fn);
	mVertex[3]=D3DXVECTOR3(x,-y,fn);
	mVertex[4]=D3DXVECTOR3(-x,-y,fn);

}

GCuller::~GCuller(void)
{

}

void GCuller::Update( D3DXMATRIX *pMat )
{
	D3DXVECTOR3 Vec[5];

	for (int i=0;i<5;i++)
	{
		D3DXVec3TransformCoord(&Vec[i],&mVertex[i],pMat);
	}

	//顶点示意图
	//1.........2
	//4.........3
	//上面
	D3DXPlaneFromPoints(&mPlane[0],&Vec[2],&Vec[1],&Vec[0]);

	//下面
	D3DXPlaneFromPoints(&mPlane[1],&Vec[0],&Vec[4],&Vec[3]);

	//左面
	D3DXPlaneFromPoints(&mPlane[2],&Vec[0],&Vec[1],&Vec[4]);

	//右面
	D3DXPlaneFromPoints(&mPlane[3],&Vec[0],&Vec[3],&Vec[2]);

	//最远面
	D3DXPlaneFromPoints(&mPlane[4],&Vec[1],&Vec[2],&Vec[3]);
}

bool GCuller::IsInEye( D3DXVECTOR3 vPos )
{

	for (int i=0;i<5;i++)
	{
		if(D3DXPlaneDotCoord(&mPlane[i],&vPos)<-50.0f)
		{
			return false;
		}
	}

	return true;
}
