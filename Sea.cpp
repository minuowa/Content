#include "GGameDemoHeader.h"
#include "Sea.h"
#include "GMeshBuffer.h"
#include "GD9Device.h"
#include "GTimer.h"

CSea::CSea ( void )
    : mCreateParam ( 0 )
{
    mQuakeCount = 0;

    mpFace = NULL;

    mCellCount = 0;
    mfMaxHeight = 0;
    mfCellWidth = 0;
    mstrFileName = NULL;
    dSafeDelete ( mCreateParam );
}

CSea::~CSea ( void )
{
}

bool CSea::reCreate()
{
    if ( !__super::reCreate() )
        return false;

    HRESULT hr = S_FALSE;

    SeaVertex *pVertextBuffer = NULL;	  //Mesh的顶点缓冲区

    DWORD *pIndexBuffer = NULL;	  //Mesh的索引缓冲区


    mMeshBufferNode = new GMeshBufferNode();
    mCellCount = mCreateParam->LnCellCount;
    mfMaxHeight = mCreateParam->mfMaxHeight;
    mfCellWidth = mCreateParam->mfCellWidth;
    mstrFileName = mCreateParam->mstrFileName;

    //SAFERELEASE(mMeshBufferNode->mpRootMesh);

    ID3DXMesh* mesh = 0;
    hr = D3DXCreateMeshFVF (
             mCellCount * mCellCount * 2,
             ( mCellCount + 1 ) * ( mCellCount + 1 ),
             D3DXMESH_32BIT | D3DXMESH_MANAGED, FVFSea, D9DEVICE->GetDvc(),
             &mesh
         );
    DebugMsgBox ( hr, "创建海面Mesh失败！" );

    DWORD dwIndex = 0;

    mesh->LockVertexBuffer ( D3DLOCK_DISCARD, ( void** ) &pVertextBuffer );

    for ( int i = 0; i < mCellCount + 1; i++ )
    {
        for ( int j = 0; j < mCellCount + 1; j++ )
        {
            dwIndex = i * ( mCellCount + 1 ) + j;

            pVertextBuffer[dwIndex].vertex.x = ( j - mCellCount / 2.0f ) * mfCellWidth;
            pVertextBuffer[dwIndex].vertex.y = 0;
            pVertextBuffer[dwIndex].vertex.z = ( i - mCellCount / 2.0f ) * mfCellWidth;
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

    mMeshBufferNode->Mesh ( mesh );
    mMeshBufferNode->SubSetCount ( 1 );

    DWORD *pAdj = new DWORD[mesh->GetNumFaces() * 3];

    mesh->GenerateAdjacency ( 1.0f, pAdj );
    GMetrialData* material = new GMetrialData;
    mMeshBufferNode->Add ( material );
    mpFace = new LPDIRECT3DTEXTURE9[1];
    material->SetTexture ( mstrFileName );

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
    material->SetMetiral ( mtrl );

    SetNormal ( mMeshBufferNode->Mesh(), D9DEVICE->GetDvc() );

    ResetVectorMesh();

    return true;

}

int CSea::AddQuakePoint ( float _x, float _z, float fAmplitude, float fAngVelocity )
{
    float t = timeGetTime() / 1000.0f;
    mQuakList[mQuakeCount++].Init ( _x, _z, fAmplitude, fAngVelocity, t );
    return mQuakeCount;
}

void CSea::update()
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

    mMeshBufferNode->Mesh()->LockVertexBuffer ( D3DLOCK_DISCARD, ( void** ) &pVertexBuffer );

    for ( int i = 0; i < mCellCount + 1; i++ )
    {
        for ( int j = 0; j < mCellCount + 1; j++ )
        {
            dwIndex = i * ( mCellCount + 1 ) + j;
            float fDelta = 0;

            for ( int k = 0; k < mQuakeCount; k++ )
            {
                fDelta += mQuakList[k].GetPointEffect ( pVertexBuffer[dwIndex].vertex.x, pVertexBuffer[dwIndex].vertex.z, ( float ) timeGetTime() / 1000.0f );
            }

            pVertexBuffer[dwIndex].vertex.y = fDelta;
        }
    }

    mMeshBufferNode->Mesh()->UnlockVertexBuffer();

    DWORD dwTime = timeGetTime();
    //sprintf(sFileName,"res\\water\\BlueShort\\A21C_%03d.jpg",);

    GString sFileName;
    sFileName.Format ( "..\\Data\\res\\water\\BlueShort\\A21C_%03d.jpg", dwTime % SEAPICNUM );
	
	GTexture* gtext=TextureMgr->getResource(sFileName);
	if (gtext)
		mpFace[0]=gtext->getTexture();
}



MeshPara* CSea::getParam() const
{
    return mCreateParam;
}

void CSea::setParam ( const MeshPara& val )
{
    dSafeDelete ( mCreateParam );
    mCreateParam = new MeshPara;
    dMemoryCopy ( mCreateParam, ( void* ) &val, sizeof ( MeshPara ) );
}





void QuakePoint::Init ( float _x, float _z, float amplitude, float AngVelocity, float _t )
{
    mfZ = _z;
    mfX = _x;

    mAmplitude = amplitude;
    mAngVelocity = AngVelocity;

    mInitTime = _t;
}

float QuakePoint::GetPointEffect ( float _x, float _z, float _t )
{
    float len = D3DXVec3Length ( & ( D3DXVECTOR3 ( mfX, 0, mfZ ) - D3DXVECTOR3 ( _x, 0, _z ) ) );

    float dt = len / QUAKE_SPEED;

    float fDeltaY = mAmplitude * sinf ( mAngVelocity * ( _t - mInitTime ) - dt );

    return fDeltaY;
}
