#include "GGameDemoHeader.h"
#include "GGameMap.h"
#include "GMeshBuffer.h"
#include "GD9Device.h"

GGameMap::GGameMap ( void )
    : _pTexture ( 0 )
    , _pMesh ( 0 )
    , _pMat ( 0 )
{
    ZeroMemory ( mppStaticObj, sizeof ( mppStaticObj ) );
}

GGameMap::~GGameMap ( void )
{
    for ( unsigned int i = 0; i < _nNumFace; ++i )
    {
        if ( _pTexture )
        {
            SAFERELEASE ( _pTexture[i] );
        }
    }

    _nNumFace = 0;
    SAFERELEASE ( _pMesh );
    SAFED_ELETE ( _pMat );
}

bool GGameMap::reCreate()
{
    if ( !__super::reCreate(  ) )
        return false;

    //获取参数

    //mMeshPara = ( MeshPara* )Para;

    //LnCellCount = mMeshPara->LnCellCount;
    //mfMaxHeight = mMeshPara->mfMaxHeight;
    //mfCellWidth = mMeshPara->mfCellWidth;
    //mstrFileName = mMeshPara->mstrFileName;
    //mstrHeightMap = mMeshPara->mstrHeightMap;

    AfterCreate();

    return true;
}

bool GGameMap::render()
{
    if ( !__super::render() )
        return false;
    D3DXMATRIX mat;
    D3DXMatrixIdentity ( &mat );
    D9DEVICE->GetDvc()->SetTransform ( D3DTS_TEXTURE0, &mat );

    return true;

}


void GGameMap::LoadFromFile ( char *sConfigName )
{
    if ( IsStrEmpty ( sConfigName ) )
    {
        return;
    }

    char sTmp[128];
    ZeroMemory ( sTmp, sizeof ( sTmp ) );

    mstrHeightMap = new char[FILE_NAME_LENGTH];
    GetPrivateProfileStringA ( "Map", "HeightMapFileName", "", mstrHeightMap, 128, sConfigName );

    mstrFileName = new char[FILE_NAME_LENGTH];
    GetPrivateProfileStringA ( "Map", "TextureFileName", "", mstrFileName, 128, sConfigName );

    GetPrivateProfileStringA ( "Map", "fMapCellWidth", "80.0", sTmp, 128, sConfigName );
    mfCellWidth = atof ( sTmp );
    ZeroMemory ( sTmp, sizeof ( sTmp ) );

    GetPrivateProfileStringA ( "Map", "fMapMaxHeight", "800.0", sTmp, 128, sConfigName );
    mfMaxHeight = atof ( sTmp );
    ZeroMemory ( sTmp, sizeof ( sTmp ) );

    LnCellCount = GetPrivateProfileIntA ( "Map", "nMapCellCount", 64, sConfigName );

    GString sName;

    for ( int i = 1; i < 10; i++ )
    {
        sName.Format ( "OBJ%d", i );
        GetPrivateProfileStringA ( "ObjList", sName, "", sTmp, 128, sConfigName );

        if ( strlen ( sTmp ) == 0 )
        {
            break;
        }

        StrToMapObjInfo ( sTmp, &mppStaticObj[i - 1] );
        sName.clear();
        ZeroMemory ( sTmp, sizeof ( sTmp ) );
    }

}


bool GGameMap::StrToMapObjInfo ( const char *str, MapObjInfo *pObjInfo )
{
    if ( IsStrEmpty ( str ) || pObjInfo == NULL )
        return false;

    sscanf_s ( str, "%d,%.f,%.f", &pObjInfo->nObjId, &pObjInfo->x, &pObjInfo->z );

    return true;
}

int GGameMap::AfterCreate()
{

    IDirect3DTexture9 *pHeightMap = NULL;	  //高度图纹理

    D3DSURFACE_DESC surfaceDesc;    //高度图纹理表面

    D3DLOCKED_RECT rcLock;	//锁定的表面

    MAPVERTEX *pVertextBuffer = NULL;	  //Mesh的顶点缓冲区

    DWORD *pIndexBuffer = NULL;	  //Mesh的索引缓冲区

    DWORD dwWidth = 0;	  //纹理宽度
    DWORD dwHeight = 0;	  //纹理高度

    HRESULT hr = -1;

    //创建Mesh

    SAFERELEASE ( _pMesh );

    hr = D3DXCreateMeshFVF (
             LnCellCount * LnCellCount * 2,
             ( LnCellCount + 1 ) * ( LnCellCount + 1 ),
             D3DXMESH_32BIT | D3DXMESH_MANAGED, FVFMAP, D9DEVICE->GetDvc(), &_pMesh
         );

    if ( FAILED ( hr ) )
    {
        return -1;
    }

    _nNumFace = 1;
    _pMat = new D3DMATERIAL9[_nNumFace];

    _pTexture = new LPDIRECT3DTEXTURE9[_nNumFace];

    //设置材质
    ZeroMemory ( _pMat, _nNumFace * sizeof ( D3DMATERIAL9 ) );

    _pMat[0].Diffuse.r = 0.4f;
    _pMat[0].Diffuse.g = 0.4f;
    _pMat[0].Diffuse.b = 0.4f;
    _pMat[0].Diffuse.a = 1.0f;
    _pMat[0].Ambient.r = 0.3f;
    _pMat[0].Ambient.g = 0.3f;
    _pMat[0].Ambient.b = 0.3f;
    _pMat[0].Ambient.a = 1.0f;
    _pMat[0].Specular.r = 0.6f;
    _pMat[0].Specular.g = 0.6f;
    _pMat[0].Specular.b = 0.6f;
    _pMat[0].Specular.a = 1.0f;
    _pMat[0].Emissive.r = 0.2f;
    _pMat[0].Emissive.g = 0.2f;
    _pMat[0].Emissive.b = 0.2f;
    _pMat[0].Emissive.a = 1.0f;
    _pMat[0].Power = 9.0f;

    //设置纹理
    if ( mstrFileName != NULL )
    {
        hr = D3DXCreateTextureFromFileA ( D9DEVICE->GetDvc(), mstrFileName, &_pTexture[0] );
    }

    //读取高度图
    if ( mstrHeightMap != NULL )
    {
        hr = D3DXCreateTextureFromFileA ( D9DEVICE->GetDvc(), mstrHeightMap, &pHeightMap );
    }

    pHeightMap->GetLevelDesc ( 0, &surfaceDesc );

    dwHeight = surfaceDesc.Height;
    dwWidth = surfaceDesc.Width;

    hr = pHeightMap->LockRect ( 0, &rcLock, NULL, 0 );

    DWORD dwPitch = rcLock.Pitch / sizeof ( DWORD );

    DWORD *pBit = ( DWORD* ) rcLock.pBits;

    _pMesh->LockVertexBuffer ( D3DLOCK_DISCARD, ( void** ) &pVertextBuffer );

    DWORD dwRow, dwLine;
    //设置顶点数据
    DWORD dwIndex;

    for ( int i = 0; i < LnCellCount + 1; i++ )
    {
        for ( int j = 0; j < LnCellCount + 1; j++ )
        {

            dwLine = dwWidth * j / ( LnCellCount + 1 );
            dwRow = dwHeight * ( LnCellCount + 1 - i ) / ( LnCellCount + 1 );

            DWORD dwInd = dwRow * dwWidth + dwLine;

            DWORD dwcolor = pBit[dwInd];
            dwcolor = dwcolor << 8;
            dwcolor = dwcolor >> 24;

            float fHeight = ( float ) dwcolor;

            dwIndex = i * ( LnCellCount + 1 ) + j;

            pVertextBuffer[dwIndex].vVertex.x = ( j - LnCellCount / 2.0f ) * mfCellWidth;
            pVertextBuffer[dwIndex].vVertex.y = ( fHeight - 255 * SEA_DEPTH_FACTOR ) / 255.0f * mfMaxHeight;
            pVertextBuffer[dwIndex].vVertex.z = ( i - LnCellCount / 2.0f ) * mfCellWidth;
            pVertextBuffer[dwIndex].u = j;
            pVertextBuffer[dwIndex].v = ( LnCellCount - i );
        }
    }

    DWORD dwVerNum = LnCellCount + 1;

    //平滑处理
    for ( int i = 1; i < LnCellCount; i++ )
    {
        for ( int j = 1; j < LnCellCount; j++ )
        {
            pVertextBuffer[i * ( LnCellCount + 1 ) + j].vVertex.y = 0.125f * (
                        pVertextBuffer[ ( i - 1 ) * dwVerNum + j - 1].vVertex.y +
                        pVertextBuffer[ ( i - 1 ) * dwVerNum + j].vVertex.y +
                        pVertextBuffer[ ( i - 1 ) * dwVerNum + j - 1].vVertex.y +
                        pVertextBuffer[i * dwVerNum + j - 1].vVertex.y +
                        pVertextBuffer[i * dwVerNum + j + 1].vVertex.y +
                        pVertextBuffer[ ( i + 1 ) * dwVerNum + j - 1].vVertex.y +
                        pVertextBuffer[ ( i + 1 ) * dwVerNum + j].vVertex.y +
                        pVertextBuffer[ ( i + 1 ) * dwVerNum + j + 1].vVertex.y
                    );
        }
    }

    pHeightMap->UnlockRect ( 0 );

    SAFERELEASE ( pHeightMap );

    _pMesh->UnlockVertexBuffer();

    //设置索引数据
    _pMesh->LockIndexBuffer ( D3DLOCK_DISCARD, ( void** ) &pIndexBuffer );

    DWORD dwBaseIndex = 0;

    for ( int i = 0; i < LnCellCount; i++ )
    {
        for ( int j = 0; j < LnCellCount; j++ )
        {
            pIndexBuffer[dwBaseIndex + 0] = i * ( LnCellCount + 1 ) + j;
            pIndexBuffer[dwBaseIndex + 1] = ( i + 1 ) * ( LnCellCount + 1 ) + j;
            pIndexBuffer[dwBaseIndex + 2] = ( i + 1 ) * ( LnCellCount + 1 ) + j + 1;

            pIndexBuffer[dwBaseIndex + 3] = i * ( LnCellCount + 1 ) + j;
            pIndexBuffer[dwBaseIndex + 4] = ( i + 1 ) * ( LnCellCount + 1 ) + j + 1;
            pIndexBuffer[dwBaseIndex + 5] = i * ( LnCellCount + 1 ) + j + 1;

            dwBaseIndex += 6;
        }
    }

    _pMesh->UnlockIndexBuffer();

    DWORD dwNum = _pMesh->GetNumFaces() * 3;
    DWORD *pAdj = new DWORD[_pMesh->GetNumFaces() * 3];

    mMeshBufferNode = new GMeshBufferNode();
    mMeshBufferNode->SubSetCount ( 1 );
    mMeshBufferNode->Mesh ( _pMesh );
    mLighting = true;

    GMetrialData* data = new GMetrialData;
    data->SetTexture ( mstrFileName );
    data->SetMetiral ( *_pMat );
    mMeshBufferNode->Add ( data );
    _pMesh->GenerateAdjacency ( 1.0f, pAdj );

    mMeshBufferNode->MakeLod ( pAdj );

    SetNormal ( mMeshBufferNode->Mesh(), D9DEVICE->GetDvc() );

    ResetVectorMesh();

    return _nID;
}

bool GGameMap::IsBlock ( D3DXVECTOR3 vPos, int *pCounter )
{
    bool bBlock = false;

    for ( int i = 0; i < 1000; i++ )
    {
        if ( mppStaticObj[i].nObjId < 1 )
        {
            return false;
        }

        D3DXVECTOR3 vMin ( mppStaticObj[i].vMin ), vMax ( mppStaticObj[i].vMax );

        if ( vPos.x > vMin.x && vPos.x < vMax.x && vPos.z > vMin.z && vPos.z < vMax.z )
        {
            if ( pCounter != NULL )
            {
                *pCounter = i;
            }

            return true;
        }
    }

    return false;
}



