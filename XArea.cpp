#include "GGameDemoHeader.h"
#include "XArea.h"

CXArea::CXArea( void )
{

}

CXArea::~CXArea( void )
{

}

D3DXVECTOR3 CXArea::GeneratePos()
{
	return ZEROVECTOR3;
}

void CXArea::Init()
{

}

bool CXArea::IsDotInArea( D3DXVECTOR3 vPos )
{
	return false;
}

D3DXVECTOR3 CAreaCube::GeneratePos()
{

	D3DXVECTOR3 vPos = ZEROVECTOR3;

	vPos.x = RandFloatEx( mfxMin, mfxMax );

	vPos.y = RandFloatEx( mfyMin, mfyMax );

	vPos.z = RandFloatEx( mfzMin, mfzMax );

	return vPos;

}





void CAreaCube::Init( float xmin, float xmax, float ymin, float ymax, float zmin, float zmax )
{
	mfxMin = xmin;
	mfxMax = xmax;

	mfyMin = ymin;
	mfyMax = ymax;

	mfzMin = zmin;
	mfzMax = zmax;

	//D3DXVECTOR3 v[8];

	////前面4点
	//v[0]=D3DXVECTOR3(mfxMin,mfyMin,mfzMin);
	//v[1]=D3DXVECTOR3(mfxMax,mfyMin,mfzMin);
	//v[2]=D3DXVECTOR3(mfxMax,mfyMax,mfzMin);
	//v[3]=D3DXVECTOR3(mfxMin,mfyMax,mfzMin);

	////后面4点
	//v[4]=D3DXVECTOR3(mfxMin,mfyMin,mfzMax);
	//v[5]=D3DXVECTOR3(mfxMax,mfyMin,mfzMax);
	//v[6]=D3DXVECTOR3(mfxMax,mfyMax,mfzMax);
	//v[7]=D3DXVECTOR3(mfxMin,mfyMax,mfzMax);

	////正面朝盒子里面
	////前后面
	//D3DXPlaneFromPoints(&mPlanes[0],&v[0],&v[1],&v[2]);
	//D3DXPlaneFromPoints(&mPlanes[1],&v[4],&v[6],&v[5]);

	////左右面
	//D3DXPlaneFromPoints(&mPlanes[2],&v[4],&v[0],&v[3]);
	//D3DXPlaneFromPoints(&mPlanes[3],&v[5],&v[2],&v[1]);

	////上下面
	//D3DXPlaneFromPoints(&mPlanes[4],&v[3],&v[2],&v[6]);
	//D3DXPlaneFromPoints(&mPlanes[5],&v[0],&v[4],&v[1]);
}

bool CAreaCube::IsDotInArea( D3DXVECTOR3 vPos )
{
	if ( vPos.x < mfxMin || vPos.x > mfxMax || vPos.y < mfyMin || vPos.y > mfyMax || vPos.z < mfzMin || vPos.z > mfzMax )
	{
		return false;
	}

	return true;
}

CAreaCube::CAreaCube()
{
	ZeroMemory( mPlanes, sizeof( mPlanes ) );
}

CAreaCube::~CAreaCube()
{

}

D3DXVECTOR3 CAreaSphere::GeneratePos()
{
	D3DXVECTOR3 vPos = ZEROVECTOR3;

	int nRadius = RandIntEx( 0, LnRadius );

	float fRad1 = RandFloatEx( -D3DX_PI / 2.0f, D3DX_PI / 2.0f );
	float fRad2 = RandFloatEx( 0, 2 * D3DX_PI );

	vPos.x = nRadius * cosf( fRad1 ) * cosf( fRad2 );
	vPos.z = nRadius * cosf( fRad1 ) * sinf( fRad2 );
	vPos.y = nRadius * sinf( fRad1 );

	return vPos;
}

void CAreaSphere::Init( int nRadius )
{
	LnRadius = nRadius;
}

CAreaSphere::CAreaSphere()
{

}

CAreaSphere::~CAreaSphere()
{

}

bool CAreaSphere::IsDotInArea( D3DXVECTOR3 vPos )
{
	if ( ( ( float )D3DXVec3Length( &vPos ) ) < LnRadius )
	{
		return true;
	}

	return false;
}

D3DXVECTOR3 CAreaSphereHollow::GeneratePos()
{
	D3DXVECTOR3 vPos = ZEROVECTOR3;

	float fRad1 = RandFloatEx( -D3DX_PI / 2.0f, D3DX_PI / 2.0f );
	float fRad2 = RandFloatEx( 0, 2 * D3DX_PI );

	vPos.x = LnRadius * cosf( fRad1 ) * cosf( fRad2 );
	vPos.z = LnRadius * cosf( fRad1 ) * sinf( fRad2 );
	vPos.y = LnRadius * sinf( fRad1 );

	return vPos;
}

void CAreaSphereHollow::Init( int nRadius )
{
	LnRadius = nRadius;
}

bool CAreaSphereHollow::IsDotInArea( D3DXVECTOR3 vPos )
{
	float fLen = ( float )D3DXVec3Length( &vPos );

	if ( fLen < LnRadius + 0.01f || fLen > LnRadius - 0.01f )
	{
		return true;
	}

	return false;
}

CAreaSphereHollow::CAreaSphereHollow()
{

}

CAreaSphereHollow::~CAreaSphereHollow()
{

}

void CAreaRect::Init( float Height, float Width )
{
	m_fWidth = Width;

	m_fHeight = Height;
}

CAreaRect::CAreaRect()
{

}

CAreaRect::~CAreaRect()
{

}

bool CAreaRect::IsDotInArea( D3DXVECTOR3 vPos )
{
	if ( vPos.x > m_fWidth || vPos.y > m_fHeight || vPos.x < 0 || vPos.y < 0 )
	{
		return false;
	}

	return true;
}

D3DXVECTOR3 CAreaRect::GeneratePos()
{
	D3DXVECTOR3 vPos = ZEROVECTOR3;

	vPos.x = RandFloatEx( 0, m_fWidth );
	vPos.y = RandFloatEx( 0, m_fHeight );
	vPos.z = 0;

	return vPos;
}
