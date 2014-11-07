#include "GGameDemoHeader.h"
#include "GBound.h"

GBound::GBound(void)
{

}

GBound::~GBound(void)
{

}

bool GBound::Init( ID3DXMesh *pMesh )
{
	return true;
}

CBoundAABB::CBoundAABB()
{

}

CBoundAABB::~CBoundAABB()
{

}



bool CBoundAABB::Init( ID3DXMesh *pMesh )
{
	GBound::Init( pMesh );

	HRESULT hr=S_FALSE;

	if (pMesh==NULL)
	{
		return false;
	}

	D3DXVECTOR3 v(ZEROVECTOR3),vMin(ZEROVECTOR3),vMax(ZEROVECTOR3);

	hr=D3DXComputeBoundingBox(&v,pMesh->GetNumVertices(),D3DXGetFVFVertexSize(pMesh->GetFVF()),&vMin,&vMax);

	if (FAILED(hr))
	{
		return false;
	}

	mArea.Init(vMin.x,vMax.x,vMin.y,vMax.y,vMin.z,vMax.z);

	return true;

}

CBoundSphere::CBoundSphere()
{
	mfRadious=ZEROFLOAT;
}

bool CBoundSphere::Init( ID3DXMesh *pMesh )
{

	if (pMesh==NULL)
	{
		return false;
	}


	D3DXVECTOR3 vCenter=ZEROVECTOR3;

	D3DXVECTOR3 *pVertexData;
	pMesh->LockVertexBuffer(0,(void**)&pVertexData);

	HRESULT hr=D3DXComputeBoundingSphere((D3DXVECTOR3*)pVertexData,pMesh->GetNumVertices(),sizeof(D3DXVECTOR3),&vCenter,&mfRadious);

	pMesh->UnlockVertexBuffer();

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}
