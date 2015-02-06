#include "GGameDemoHeader.h"
#include "GWater.h"
#include "GMeshBuffer.h"
#include "GD9Device.h"
#include "GTimer.h"

GWater::GWater ( void )
{
	mQuakeCount = 3;

	mCellCount = 0;
	mMaxHeight = 0;
	mCellWidth = 0;
	mQuakeAngleVelocity = 1;
}

GWater::~GWater ( void )
{
	clear();
}

bool GWater::recreate()
{
	if ( !__super::recreate() )
		return false;

	mMeshBufferNode = new GMeshBufferNode();

	recreateGraphInfo();

	recreateQuakeInfo();

	recreateMetrialInfo();

	dSetMeshNormal ( mMeshBufferNode->getMesh(), D9Device->GetDvc() );

	recreateInsectMesh();

	return true;

}

int GWater::addQuakePoint ( float _x, float _z, float fAmplitude, float fAngVelocity )
{
	QuakePoint* quake = new QuakePoint;
	mQuakeManager.addPoint ( quake );
	float t = timeGetTime() / 1000.0f;
	quake->init ( _x, _z, fAmplitude, fAngVelocity, t );
	return mQuakeCount;
}

void GWater::update()
{

	//海面纹理按时间切换

	static DWORD dwMainTime = 0;
	static DWORD dwPicIndex = 0;

	dwMainTime += TheTimer->getFrameTimems();

	if ( dwMainTime > dwPicIndex * FaceSwitchTime )
	{
		dwPicIndex++;
	}

	if ( dwPicIndex > SEAPICNUM - 1 )
	{
		dwMainTime = 0;
		dwPicIndex = 0;
	}


	HRESULT hr = S_FALSE;

	SeaVertex *pVertexBuffer;

	DWORD dwIndex = 0;

	mMeshBufferNode->getMesh()->LockVertexBuffer ( D3DLOCK_DISCARD, ( void** ) &pVertexBuffer );

	for ( int i = 0; i < mCellCount + 1; i++ )
	{
		for ( int j = 0; j < mCellCount + 1; j++ )
		{
			dwIndex = i * ( mCellCount + 1 ) + j;

			float fDelta = mQuakeManager.getPointEffect ( pVertexBuffer[dwIndex].vertex.x, pVertexBuffer[dwIndex].vertex.z, timeGetTime() );

			pVertexBuffer[dwIndex].vertex.y = fDelta;
		}
	}

	mMeshBufferNode->getMesh()->UnlockVertexBuffer();

	DWORD dwTime = timeGetTime();
	//sprintf(sFileName,"res\\water\\BlueShort\\A21C_%03d.jpg",);

	GString sFileName;
	sFileName.Format ( "..\\Data\\res\\water\\BlueShort\\A21C_%03d.jpg", dwTime / 100 % SEAPICNUM );

	GMetrialData* metrial = mMeshBufferNode->getMaterial ( 0 );
	CXASSERT ( metrial );
	metrial->setTexture ( sFileName );
}




void GWater::setParam ( const MeshPara& val )
{
	mCellCount = val.mCellCount;
	mMaxHeight = val.mMaxHeight;
	mCellWidth = val.mCellWidth;
	mTextureFile = val.mFileName;
}

void GWater::clear()
{
	dSafeDelete ( mMeshBufferNode );
}

void GWater::registerAllProperty()
{
	__super::registerAllProperty();

	__RegisterProperty ( mCellCount );
	__RegisterProperty ( mMaxHeight );
	__RegisterProperty ( mCellWidth );
	__RegisterProperty ( mTextureFile );
	__RegisterProperty ( mQuakeCount );
	__RegisterProperty ( mQuakeAngleVelocity );
}


void GWater::onPropertyChangeEnd ( void* cur )
{
	__super::onPropertyChangeEnd ( cur );

	if (
		cur == &mCellCount
		|| cur == &mCellWidth
		|| cur == &mQuakeCount
		|| cur == &mQuakeAngleVelocity
		)
	{
		recreate();
	}
}

void GWater::recreateQuakeInfo()
{
	float dim = mCellWidth * mCellCount;
	mQuakeManager.setRangeX ( -dim / 2, dim / 2 );
	mQuakeManager.setRangeZ ( -dim / 2, dim / 2 );

	mQuakeManager.recreate ( mQuakeCount, mQuakeAngleVelocity );
}

void GWater::recreateGraphInfo()
{
	HRESULT hr = S_FALSE;

	SeaVertex *pVertextBuffer = NULL;	  //Mesh的顶点缓冲区

	DWORD *pIndexBuffer = NULL;	  //Mesh的索引缓冲区
	ID3DXMesh* mesh = 0;
	hr = D3DXCreateMeshFVF (
		mCellCount * mCellCount * 2,
		( mCellCount + 1 ) * ( mCellCount + 1 ),
		D3DXMESH_32BIT | D3DXMESH_MANAGED, FVFSea, D9Device->GetDvc(),
		&mesh
		);

	dDebugMsgBox ( hr, "创建海面Mesh失败！" );

	DWORD dwIndex = 0;

	mesh->LockVertexBuffer ( D3DLOCK_DISCARD, ( void** ) &pVertextBuffer );

	for ( int i = 0; i < mCellCount + 1; i++ )
	{
		for ( int j = 0; j < mCellCount + 1; j++ )
		{
			dwIndex = i * ( mCellCount + 1 ) + j;

			pVertextBuffer[dwIndex].vertex.x = ( j - mCellCount / 2.0f ) * mCellWidth;
			pVertextBuffer[dwIndex].vertex.y = 0;
			pVertextBuffer[dwIndex].vertex.z = ( i - mCellCount / 2.0f ) * mCellWidth;
			pVertextBuffer[dwIndex].u = j / 10.0f;
			pVertextBuffer[dwIndex].v = ( mCellCount - i ) / 10.0f;
		}
	}

	mesh->UnlockVertexBuffer();

	mesh->LockIndexBuffer ( D3DLOCK_DISCARD, ( void** ) &pIndexBuffer );

	DWORD dwBaseIndex = 0;

	for ( int i = 0; i < mCellCount; i++ )
	{
		for ( int j = 0; j < mCellCount; j++ )
		{
			pIndexBuffer[dwBaseIndex + 0] = i * ( mCellCount + 1 ) + j;
			pIndexBuffer[dwBaseIndex + 1] = ( i + 1 ) * ( mCellCount + 1 ) + j;
			pIndexBuffer[dwBaseIndex + 2] = ( i + 1 ) * ( mCellCount + 1 ) + j + 1;

			pIndexBuffer[dwBaseIndex + 3] = i * ( mCellCount + 1 ) + j;
			pIndexBuffer[dwBaseIndex + 4] = ( i + 1 ) * ( mCellCount + 1 ) + j + 1;;
			pIndexBuffer[dwBaseIndex + 5] = i * ( mCellCount + 1 ) + j + 1;

			dwBaseIndex += 6;
		}
	}

	mesh->UnlockIndexBuffer();

	mMeshBufferNode->setMesh ( mesh );
	mMeshBufferNode->setSubCount ( 1 );

	DWORD *pAdj = new DWORD[mesh->GetNumFaces() * 3];

	mesh->GenerateAdjacency ( 1.0f, pAdj );

	dSafeDeleteArray ( pAdj );
}

void GWater::recreateMetrialInfo()
{
	GMetrialData* material = new GMetrialData;
	mMeshBufferNode->Add ( material );
	material->setTexture ( mTextureFile );

	D3DMATERIAL9 mtrl;
	ZeroMemory ( &mtrl, sizeof ( mtrl ) );

	mtrl.Diffuse.r = 0;
	mtrl.Diffuse.g = 1;
	mtrl.Diffuse.b = 0;
	mtrl.Diffuse.a = 0.5;
	mtrl.Ambient.r = 0;
	mtrl.Ambient.g = 0;
	mtrl.Ambient.b = 0;
	mtrl.Ambient.a = 0.5;
	mtrl.Specular.r = 0;
	mtrl.Specular.g = 0;
	mtrl.Specular.b = 0;
	mtrl.Specular.a = 0.5;
	mtrl.Emissive.r = 0;
	mtrl.Emissive.g = 1;
	mtrl.Emissive.b = 0.8;
	mtrl.Emissive.a = 0.5;

	mtrl.Power = 9.0f;
	material->setMetiral ( mtrl );
}

void GWater::setPointCount ( unsigned int cnt )
{
	mQuakeCount = cnt;
	recreateQuakeInfo();
}

bool GWater::render()
{
	D9Device->openAllLight ( true, false );
	D9Device->openAlphaBlend(true);
	return __super::render();
}





void QuakePoint::init ( float _x, float _z, float amplitude, float AngVelocity, DWORD _t )
{
	mfZ = _z;
	mfX = _x;

	mAmplitude = amplitude;
	mAngVelocity = AngVelocity;

	mInitTime = _t;
}
#define QUAKE_SPEED 180.0f						//波在水中传播的速度

float QuakePoint::getPointEffect ( float _x, float _z, DWORD _t )
{
	float len = D3DXVec3Length ( & ( D3DXVECTOR3 ( mfX, 0, mfZ ) - D3DXVECTOR3 ( _x, 0, _z ) ) );

	float dt = len / QUAKE_SPEED;
	static int maxDistancte = 8000;
	float reduceFactor = len / maxDistancte;
	float res = ( 1 - reduceFactor * reduceFactor );
	float fDeltaY = 0;
	if ( res > 0 )
		fDeltaY = res * mAmplitude * sinf ( mAngVelocity * ( _t - mInitTime ) * 0.001f - dt );

	return fDeltaY;
}

void GQuakePointManager::addPoint ( QuakePoint* point )
{
	mPointList.push_back ( point );
}

void GQuakePointManager::recreate ( CXIndex cnt , float angVelocity )
{
	destory();

	for ( CXIndex i = 0; i < cnt; ++i )
	{
		QuakePoint* quake = new QuakePoint;
		quake->init ( gRandom.randF ( mMinX, mMaxX ), gRandom.randF ( mMinZ, mMaxZ ), 30, angVelocity, timeGetTime() );
		addPoint ( quake );
	}
}

void GQuakePointManager::destory()
{
	dSafeDeleteVector ( mPointList );
}

float GQuakePointManager::getPointEffect ( float _x, float _z, DWORD _t )
{
	float sum = 0;
	for ( QuakePoint * quake: mPointList )
	{
		sum += quake->getPointEffect ( _x, _z, _t );
	}
	return sum;
}

GQuakePointManager::GQuakePointManager()
{
	mMinX = mMinZ = -200.0f;
	mMaxX = mMaxZ = 200.0f;
}

GQuakePointManager::~GQuakePointManager()
{
	destory();
}

void GQuakePointManager::setRangeX ( float fmin, float fmax )
{
	mMinX = fmin;
	mMaxX = fmax;
}

void GQuakePointManager::setRangeZ ( float fmin, float fmax )
{
	mMinZ = fmin;
	mMaxZ = fmax;
}
