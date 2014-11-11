#include "GGameDemoHeader.h"
#include "GEffect.h"
#include <string>
#include "GD9Device.h"

GEffect gEffect;

GEffect::GEffect ( void )
{
	//m_pVDel=NULL;

	mEffectPool = NULL;

	mD3DEffect = NULL;

}

GEffect::~GEffect ( void )
{
	dSafeRelease ( mD3DEffect );
	dSafeRelease ( mEffectPool );
}



//bool GEffect::Create ( char *sEffectFileName )
//{
//	if ( IsStrEmpty ( sEffectFileName ) )
//	{
//		return false;
//	}
//
//
//	//m_pEffect->ApplyParameterBlock(m_hParaBlock);
//
//
//	//m_pEffect->GetParameterByName()
//
//	CHECK_RESULT_BOOL ( hr );
//
//	D3DVERTEXELEMENT9 decl[] =
//	{
//		{ 0, 0,   D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
//		{ 0, 12,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
//		{ 0, 24,  D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
//		D3DDECL_END()
//	};
//
//	hr = D9DEVICE->GetDvc()->CreateVertexDeclaration ( decl, &m_pVDel );
//
//	return true;
//}

D3DXHANDLE GEffect::SetEffectPara ( SetPara FunSetPara )
{
	mD3DEffect->BeginParameterBlock();

	D3DXHANDLE GHandle = FunSetPara ( mD3DEffect );

	mD3DEffect->EndParameterBlock();

	return GHandle;
}

bool GEffect::createFromFile( const char* name )
{

	HRESULT hr = E_FAIL;

	ID3DXBuffer *pErrBuffer = NULL;

	//创建顶点声明对象


	hr = D3DXCreateEffectPool ( &mEffectPool );
	CHECK_RESULT_BOOL ( hr );


	hr = D3DXCreateEffectFromFileA (
		D9DEVICE->GetDvc(),
		name,
		NULL,
		NULL,
		D3DXSHADER_DEBUG/*|D3DXSHADER_USE_LEGACY_D3DX9_31_DLL|D3DXFX_NOT_CLONEABLE*/,
		mEffectPool,
		&mD3DEffect,
		&pErrBuffer
		);

	if ( pErrBuffer != NULL )
	{
		std::string sError;
		sError.assign ( ( char* ) ( char* ) pErrBuffer->GetBufferPointer() );
		MessageBoxA ( NULL, ( char* ) pErrBuffer->GetBufferPointer(), 0, 0 );
	}

}

ID3DXEffectPool * GEffect::mEffectPool=nullptr;



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
