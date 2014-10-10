#include "GGameDemoHeader.h"


D3DVECTOR Vector ( float x, float y, float z )
{
    D3DVECTOR v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}



void DebugMsgBox ( HRESULT hr, const char* StrDebug )
{
#ifdef _DEBUG
    if ( FAILED ( hr ) )
    {
        MessageBoxA ( NULL, StrDebug, NULL, MB_OK );
    }
#endif
}

ID3DXMesh * SetNormal ( ID3DXMesh *pMesh, IDirect3DDevice9 *DVC )
{

    HRESULT hr = S_FALSE;

    ID3DXMesh *pTemp = NULL;

    DWORD dwFVF = pMesh->GetFVF();

    if ( ! ( dwFVF & D3DFVF_NORMAL ) )
    {
        pMesh->CloneMeshFVF ( pMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, DVC, &pTemp );

        if ( pTemp != NULL )
        {
            //SAFERELEASE(pMesh);
            pMesh = pTemp;
        }
    }

    hr = D3DXComputeNormals ( pMesh, NULL );
    return pMesh;
}
int GetSubCharCount( const char* Sourc,const char* Dev )
{
    if ( ( !Sourc ) || ( !Dev ) )
    {
        return 0;
    }
    int iLen = strlen ( Sourc );
    int iSubLen = strlen ( Dev );
    int iCount = 0;
    for ( int i = 0; i < iLen - iSubLen + 1; i++ )
    {
        if ( Sourc[i] == Dev[0] )
        {
            int j = 0;
            for ( j = 1; j < iSubLen; j++ )
            {
                if ( Sourc[i + j] != Dev[j] )
                {
                    break;
                }
            }
            if ( j < iSubLen )
            {
                continue;
            }
            iCount++;
            i += iSubLen - 1;
        }
    }
    return iCount;
}

int GetSubCharIndexByCount( const char* Sourc,const char* Dev, int Count )
{
    if ( ( !Sourc ) || ( !Dev ) )
    {
        return -1;
    }
    if ( Count == 0 )
    {
        return 0;
    }
    int iLen = strlen ( Sourc );
    int iSubLen = strlen ( Dev );
    int iCount = 0;
    for ( int i = 0; i < iLen - iSubLen + 1; i++ )
    {
        if ( Sourc[i] == Dev[0] )
        {
            int j = 0;
            for ( j = 1; j < iSubLen; j++ )
            {
                if ( Sourc[i + j] != Dev[j] )
                {
                    break;
                }
            }
            if ( j < iSubLen )
            {
                continue;
            }
            iCount++;
            if ( iCount == Count )
            {
                return i;
            }
            i += iSubLen - 1;
        }
    }
    return -1;

}

bool IsStrEmpty ( const char * str )
{
    if ( str == nullptr )
    {
        return true;
    }

    if ( strlen ( str ) == 0 )
    {
        return true;
    }

    return false;
}

int RandIntEx ( int nMin, int nMax )
{
    return rand() % ( nMax - nMin + 1 ) + nMin;
}

float RandFloatEx ( float fMin, float fMax )
{
    int nMin = ( int ) ( fMin * 1000 );
    int nMax = ( int ) ( fMax * 1000 );

    int nSult = RandIntEx ( nMin, nMax );

    return ( float ) ( nSult / 1000.0f );
}

void PreRandom()
{
    LARGE_INTEGER lInt;

    QueryPerformanceCounter ( &lInt );

    srand ( lInt.LowPart );
}

bool IsFileExist ( const  char * sFileName )
{
    return ( bool ) PathFileExistsA ( sFileName );
}

void StrToVector3( const char *str ,D3DXVECTOR3& v )
{
    if ( IsStrEmpty ( str ) )
        return ;

	sscanf_s(str,"%.f,%.f,%.f",&v.x,&v.y,&v.z);
}

bool IsPointInRect ( POINT pt, RECT *rc )
{
    if ( rc == NULL )
    {
        return false;
    }

    if ( pt.x < rc->left || pt.x > rc->right || pt.y < rc->top || pt.y > rc->bottom )
    {
        return false;
    }

    return true;
}

bool GetBoundRadius ( ID3DXMesh *pMesh, float *pfRadiusOut )
{
    if ( pMesh == NULL )
    {
        return false;
    }

    D3DXVECTOR3 vCenter = ZEROVECTOR3;

    D3DXVECTOR3 *pVertexData;
    pMesh->LockVertexBuffer ( 0, ( void** ) &pVertexData );

    HRESULT hr = D3DXComputeBoundingSphere ( ( D3DXVECTOR3* ) pVertexData, pMesh->GetNumVertices(), sizeof ( D3DXVECTOR3 ), &vCenter, pfRadiusOut );

    pMesh->UnlockVertexBuffer();

    if ( FAILED ( hr ) )
    {
        return false;
    }

    return true;
}

extern const char* GetMediaPath()
{
    return "..\\Data\\";
}
