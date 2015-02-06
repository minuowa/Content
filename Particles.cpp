#include "GGameDemoHeader.h"
#include "Particles.h"
#include "GTimer.h"
#include "GD9Device.h"

CRectMesh::CRectMesh( void )
{

	//mvPosOrigin=ZEROVECTOR3;

	LnNumParticles = 0;

	mpVB = NULL;

	mpFace = NULL;

	pSnow = NULL;
}

CRectMesh::~CRectMesh( void )
{

}



bool CRectMesh::recreate()
{

	HRESULT hr = S_FALSE;

	//创建顶点缓存并初始化顶点数据

	hr = D9Device->GetDvc()->CreateVertexBuffer( LnNumParticles * sizeof( VertexSnow ), 0, D3DFVF_SNOW, D3DPOOL_DEFAULT, &mpVB, NULL );

	if ( FAILED( hr ) )
	{
		return false;
	}

	VertexSnow vertex[] =
	{
		{ -1.0f, 0.0f, 0.0f, 0.0f, 1.0f},

		{ -1.0f, 2.0f, 0.0f, 0.0f, 0.0f},

		{1.0f, 0.0f, 0.0f, 1.0f, 1.0f},

		{1.0f, 2.0f, 0.0f, 1.0f, 0.0f},
	};

	void *pVertex;

	hr = mpVB->Lock( 0, 4 * sizeof( VertexSnow ), ( void** )&pVertex, 0 );

	if ( FAILED( hr ) )
	{
		return false;
	}

	memcpy( pVertex, vertex, sizeof( vertex ) );

	mpVB->Unlock();

	//加载雪花纹理

	hr = D3DXCreateTextureFromFileA( D9Device->GetDvc(), "..\\Data\\res\\Particles\\snow\\snow.tga", &mpFace );

	dDebugMsgBox( hr, "..\\Data\\res\\Particles\\snow\\snow.tga找不到！" );

	//为每个粒子赋初始随即值

	mAreaSphere.Init( 600 );


	//加载10W个用时171ms，平均每个耗时0.00171ms
	for ( int i = 0; i < LnNumParticles; i++ )
	{
		pSnow[i].Translate = mAreaSphere.GeneratePos();

		pSnow[i].Rotate.x = gRandom.randF( ZEROFLOAT, 2 * D3DX_PI );
		pSnow[i].Rotate.y = gRandom.randF( ZEROFLOAT, 2 * D3DX_PI );
		pSnow[i].Rotate.z = ZEROFLOAT;

		pSnow[i].Speed = D3DXVECTOR3( gRandom.randI() % 3 - 3, gRandom.randI() % 3 - 3, gRandom.randI() % 3 - 3 );

		pSnow[i].SpeedRotate = 1.0f + gRandom.randI() % 10 / 10.0f;

	}

	return S_OK;
}



void CRectMesh::update( )
{
	float fPass = TheTimer->getFrameTimems() / 1000.0f;

	for ( int i = 0; i < LnNumParticles; i++ )
	{
		pSnow[i].Translate += pSnow[i].Speed * fPass / 1000.0f;

		if ( !mAreaSphere.IsDotInArea( pSnow[i].Translate ) )
		{
			pSnow[i].Translate = mAreaSphere.GeneratePos();
		}

		pSnow[i].Rotate.x += pSnow[i].SpeedRotate * fPass / 1000.0f;
		pSnow[i].Rotate.y += pSnow[i].SpeedRotate * fPass / 1000.0f;
		//pSnow[i].vRotationNow.z+=pSnow[i].fSpeedRotation*fPass/1000.0f;

	}

	GRenderEntity::update();
}

bool CRectMesh::render()
{
	return false;

	if ( mpFace != NULL )
	{
		D9Device->GetDvc()->SetTexture( 0, mpFace );
	}

	D9Device->GetDvc()->SetFVF( D3DFVF_SNOW );


	D3DXMATRIX matTranslation = NORMALMATRIX;
	D3DXMATRIX matRx, matRy, matWorld, matTmp;

	//matTmp = GNode::updateWorld( false );

	D9Device->openAllLight( false );
	D9Device->openAlphaBlend( true );

	for ( int i = 0; i < LnNumParticles; i++ )
	{

		D3DXMatrixRotationX( &matRx, pSnow[i].Rotate.x );

		D3DXMatrixRotationY( &matRy, pSnow[i].Rotate.y );

		matTranslation._41 = pSnow[i].Translate.x;

		matTranslation._42 = pSnow[i].Translate.y;

		matTranslation._43 = pSnow[i].Translate.z;

		matWorld = matRx * matRy * matTranslation * matTmp;
		CXASSERT(0);
		D9Device->GetDvc()->SetTransform( D3DTS_WORLD, &matWorld );

		D9Device->GetDvc()->SetStreamSource( 0, mpVB, 0, sizeof( VertexSnow ) );

		D9Device->GetDvc()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

	}

	D9Device->openAlphaBlend( false );

	return true;

}

