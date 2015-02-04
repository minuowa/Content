#include "common.h"
#include "XBase.h"
#include <stdio.h>

void dVector ( OUT D3DVECTOR& v, float x, float y, float z )
{
    v.x = x;
    v.y = y;
    v.z = z;
}



void dDebugMsgBox ( HRESULT hr, const char* StrDebug )
{
#ifdef _DEBUG
    if ( FAILED ( hr ) )
    {
        MessageBoxA ( NULL, StrDebug, NULL, MB_OK );
    }
#endif
}

ID3DXMesh * dSetMeshNormal ( ID3DXMesh *pMesh, IDirect3DDevice9 *DVC )
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
int GetSubCharCount ( const char* Sourc, const char* Dev )
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

int GetSubCharIndexByCount ( const char* Sourc, const char* Dev, int Count )
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

bool dIsStrEmpty ( const char * str )
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


bool dIsFileExist ( const  char * sFileName )
{
    return ( bool ) PathFileExistsA ( sFileName );
}

void dStrToVector3 ( const char *str , D3DXVECTOR3& v )
{
    if ( dIsStrEmpty ( str ) )
        return ;

    sscanf_s ( str, "%.f,%.f,%.f", &v.x, &v.y, &v.z );
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

bool dGetBoundRadius ( ID3DXMesh *pMesh, float *pfRadiusOut )
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

extern const char* fiGetMediaPath()
{
    return "..\\Data\\";
}

void dGetVector3Ease ( D3DXVECTOR3& out, D3DXVECTOR3* start, D3DXVECTOR3* end, DWORD elpaseTime, DWORD totalTime )
{
    float e = elpaseTime * 0.001f;
    float t = totalTime * 0.001f;
    out.x = gEaser.getEase ( start->x, end->x, e, t );
    out.y = gEaser.getEase ( start->y, end->y, e, t );
    out.z = gEaser.getEase ( start->z, end->z, e, t );
}

void dGetYawPitchRollFromMatrix ( const D3DXMATRIX& mat, float& roll, float& pitch, float& yaw )
{
    // Assuming the angles are in radians.
    float sp = -mat._23;
    if ( sp <= -1.0f )
    {
        pitch = -D3DX_PI / 2;
    }
    else if ( sp > 1.0f )
    {
        pitch = D3DX_PI / 2;
    }
    else
    {
        pitch = -asin ( sp );
    }
    if ( sp > 0.99998 ) // singularity at north pole
    {
        roll = 0.0f;
        yaw = atan2 ( -mat._31, mat._11 );
    }
    else
    {
        yaw = atan2 ( mat._13, mat._33 );
        roll = atan2 ( mat._21, mat._22 );
    }
}

void dGetTranslateFromMatrix ( D3DXVECTOR3& out, const D3DXMATRIX* mat )
{
    out.x = mat->_41;
    out.y = mat->_42;
    out.z = mat->_43;
}

void dMakeColor ( D3DCOLORVALUE& out, byte a, byte r, byte g, byte b )
{
    out.a = ( float ) a / 255;
    out.r = ( float ) r / 255;
    out.g = ( float ) g / 255;
    out.b = ( float ) b / 255;
}

void dVector3Cross ( D3DXVECTOR3& out, const D3DXVECTOR3& v1, const D3DXVECTOR3& v2 )
{
    D3DXVec3Cross ( &out, &v1, &v2 );
}

void dVector3Normalize ( D3DXVECTOR3& v )
{
    D3DXVec3Normalize ( &v, &v );
}

D3DXVECTOR3 dVector3NormalizeSlow ( const D3DXVECTOR3& v )
{
    D3DXVECTOR3 out;
    D3DXVec3Normalize ( &out, &v );
    return out;
}

void dMatrixInverse ( D3DXMATRIX& mat )
{
    D3DXMatrixInverse ( &mat, 0, &mat );
}

void dMatrixTranslateVec3 ( D3DXVECTOR3& outin, const D3DXMATRIX& mat )
{
    D3DXVec3TransformCoord ( &outin, &outin, &mat );
}

float dVector3Length ( const D3DXVECTOR3& v )
{
    return sqrt ( dSequare ( v.x ) + dSequare ( v.y ) + dSequare ( v.z ) );
}

float dVector3Dot ( const D3DXVECTOR3& v1, const D3DXVECTOR3& v2 )
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

void dGetMatixFromQuation ( D3DXMATRIX&out, D3DXQUATERNION& in )
{
    float x2 = in.x * in.x;
    float y2 = in.y * in.y;
    float z2 = in.z * in.z;
    float xy = in.x * in.y;
    float xz = in.x * in.z;
    float yz = in.y * in.z;
    float wx = in.w * in.x;
    float wy = in.w * in.y;
    float wz = in.w * in.z;

    out._11 = 1.0f - 2.0f * ( y2 + z2 );
    out._12 = 2.0f * ( xy + wz );
    out._13 = 2.0f * ( xz - wy );
    out._14 = 0.0f;

    out._21 = 2.0f * ( xy - wz );
    out._22 = 1.0f - 2.0f * ( x2 - z2 );
    out._23 = 2.0f * ( yz + wx );
    out._24 = 0.0f;

    out._31 = 2.0f * ( xz + wy );
    out._32 = 2.0f * ( yz - wx );
    out._33 = 1.0f - 2.0f * ( x2 + y2 );
    out._34 = 0.0f;

    out._41 = 0.0f;
    out._42 = 0.0f;
    out._43 = 0.0f;
    out._44 = 1.0f;
}
