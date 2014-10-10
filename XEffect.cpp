#include "GGameDemoHeader.h"
#include "XEffect.h"
#include <string>
#include "GD9Device.h"

CXEffect gEffect;

CXEffect::CXEffect ( void )
{
    //m_pVDel=NULL;

    m_pEffectPool = NULL;

    mD3DEffect = NULL;

}

CXEffect::~CXEffect ( void )
{
    dSafeRelease ( mD3DEffect );
    dSafeRelease ( m_pEffectPool );
}



bool CXEffect::Create ( char *sEffectFileName )
{
    if ( IsStrEmpty ( sEffectFileName ) )
    {
        return false;
    }

    HRESULT hr = E_FAIL;

    ID3DXBuffer *pErrBuffer = NULL;

    //创建顶点声明对象


    hr = D3DXCreateEffectPool ( &m_pEffectPool );
    CHECK_RESULT_BOOL ( hr );


    hr = D3DXCreateEffectFromFileA (
             D9DEVICE->GetDvc(),
             sEffectFileName,
             NULL,
             NULL,
             D3DXSHADER_DEBUG/*|D3DXSHADER_USE_LEGACY_D3DX9_31_DLL|D3DXFX_NOT_CLONEABLE*/,
             m_pEffectPool,
             &mD3DEffect,
             &pErrBuffer
         );

    if ( pErrBuffer != NULL )
    {
        std::string sError;
        sError.assign ( ( char* ) ( char* ) pErrBuffer->GetBufferPointer() );
        MessageBoxA ( NULL, ( char* ) pErrBuffer->GetBufferPointer(), 0, 0 );
    }


    //m_pEffect->ApplyParameterBlock(m_hParaBlock);


    //m_pEffect->GetParameterByName()

    CHECK_RESULT_BOOL ( hr );

    D3DVERTEXELEMENT9 decl[] =
    {
        { 0, 0,   D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        { 0, 12,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
        { 0, 24,  D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
        D3DDECL_END()
    };

    hr = D9DEVICE->GetDvc()->CreateVertexDeclaration ( decl, &m_pVDel );

    return true;
}

D3DXHANDLE CXEffect::SetEffectPara ( SetPara FunSetPara )
{
    mD3DEffect->BeginParameterBlock();

    D3DXHANDLE GHandle = FunSetPara ( mD3DEffect );

    mD3DEffect->EndParameterBlock();

    return GHandle;
}



D3DXHANDLE SetEffectParaments ( ID3DXEffect *pEffect )
{
    if ( pEffect == NULL )
    {
        return NULL;
    }

    D3DXHANDLE hWorldViewProj = NULL;

    pEffect->GetParameterByName ( hWorldViewProj, "matWorldViewProj" );

    return hWorldViewProj ;
}
