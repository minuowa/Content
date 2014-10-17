#include "GGameDemoHeader.h"
#include "GWater.h"
#include "GMeshBuffer.h"
#include "GD9Device.h"
#include "GTimer.h"

GWater::GWater ( void )
{
    mQuakeCount = 1;

    mpFace = NULL;

    mCellCount = 0;
    mMaxHeight = 0;
    mCellWidth = 0;
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

    dSetMeshNormal ( mMeshBufferNode->getMesh(), D9DEVICE->GetDvc() );

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

    //����������ʱ���л�

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

            float fDelta = mQuakeManager.getPointEffect ( pVertexBuffer[dwIndex].vertex.x, pVertexBuffer[dwIndex].vertex.z, ( float ) timeGetTime() / 1000.0f );

            pVertexBuffer[dwIndex].vertex.y = fDelta;
        }
    }

    mMeshBufferNode->getMesh()->UnlockVertexBuffer();

    DWORD dwTime = timeGetTime();
    //sprintf(sFileName,"res\\water\\BlueShort\\A21C_%03d.jpg",);

    GString sFileName;
    sFileName.Format ( "..\\Data\\res\\water\\BlueShort\\A21C_%03d.jpg", dwTime % SEAPICNUM );

    GTexture* gtext = TextureMgr->getResource ( sFileName );
    if ( gtext )
        mpFace[0] = gtext->getTexture();
}




void GWater::setParam ( const MeshPara& val )
{
    mCellCount = val.LnCellCount;
    mMaxHeight = val.mfMaxHeight;
    mCellWidth = val.mfCellWidth;
    mTextureFile = val.mstrFileName;
}

void GWater::clear()
{
    dSafeDelete ( mpFace );
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
}


void GWater::onPropertyChangeEnd ( void* cur )
{
    __super::onPropertyChangeEnd ( cur );

    if (
        cur == &mCellCount
        || cur == &mCellWidth
        || cur == &mQuakeCount
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

    mQuakeManager.recreate ( mQuakeCount );
}

void GWater::recreateGraphInfo()
{
    HRESULT hr = S_FALSE;

    SeaVertex *pVertextBuffer = NULL;	  //Mesh�Ķ��㻺����

    DWORD *pIndexBuffer = NULL;	  //Mesh������������
    ID3DXMesh* mesh = 0;
    hr = D3DXCreateMeshFVF (
             mCellCount * mCellCount * 2,
             ( mCellCount + 1 ) * ( mCellCount + 1 ),
             D3DXMESH_32BIT | D3DXMESH_MANAGED, FVFSea, D9DEVICE->GetDvc(),
             &mesh
         );
    DebugMsgBox ( hr, "��������Meshʧ�ܣ�" );

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
    mpFace = new LPDIRECT3DTEXTURE9[1];
    material->setTexture ( mTextureFile );

    D3DMATERIAL9 mtrl;
    ZeroMemory ( &mtrl, sizeof ( mtrl ) );

    mtrl.Diffuse.r = 0.4f;
    mtrl.Diffuse.g = 0.4f;
    mtrl.Diffuse.b = 1.0f;
    mtrl.Diffuse.a = 0.6f;
    mtrl.Ambient.r = 0.2f;
    mtrl.Ambient.g = 0.2f;
    mtrl.Ambient.b = 0.2f;
    mtrl.Ambient.a = 0.5f;
    mtrl.Specular.r = 0.8f;
    mtrl.Specular.g = 0.8f;
    mtrl.Specular.b = 0.8f;
    mtrl.Specular.a = 0.5f;
    mtrl.Emissive.r = 0.2f;
    mtrl.Emissive.g = 0.2f;
    mtrl.Emissive.b = 0.6f;
    mtrl.Emissive.a = 0.5f;

    mtrl.Power = 9.0f;
    material->setMetiral ( mtrl );
}

void GWater::setPointCount ( unsigned int cnt )
{
    mQuakeCount = cnt;
    recreateQuakeInfo();
}





void QuakePoint::init ( float _x, float _z, float amplitude, float AngVelocity, float _t )
{
    mfZ = _z;
    mfX = _x;

    mAmplitude = amplitude;
    mAngVelocity = AngVelocity;

    mInitTime = _t;
}

float QuakePoint::getPointEffect ( float _x, float _z, float _t )
{
    float len = D3DXVec3Length ( & ( D3DXVECTOR3 ( mfX, 0, mfZ ) - D3DXVECTOR3 ( _x, 0, _z ) ) );

    float dt = len / QUAKE_SPEED;

    float fDeltaY = mAmplitude * sinf ( mAngVelocity * ( _t - mInitTime ) - dt );

    return fDeltaY;
}

void GQuakePointManager::addPoint ( QuakePoint* point )
{
    mPointList.push_back ( point );
}

void GQuakePointManager::recreate ( CXIndex cnt )
{
    for ( CXIndex i = 0; i < cnt; ++i )
    {
        QuakePoint* quake = new QuakePoint;
        quake->init ( gRandom.randF ( mMinX, mMaxX ), gRandom.randF ( mMinZ, mMaxZ ), 50, 3.8f, timeGetTime() );
        addPoint ( quake );
    }
}

void GQuakePointManager::destory()
{
    dSafeDeleteVector ( mPointList );
}

float GQuakePointManager::getPointEffect ( float _x, float _z, float _t )
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