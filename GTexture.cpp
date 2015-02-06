#include "GGameDemoHeader.h"
#include "GTexture.h"
#include "GD9Device.h"

GTexture::GTexture()
    : mD3DTexture ( 0 )
{

}

bool GTexture::createFromFile ( const char* name )
{
    mFileName = name;
    dSafeRelease ( mD3DTexture );
    D3DXCreateTextureFromFileExA (
        D9Device->GetDvc(),
        name,
        D3DX_FROM_FILE,
        D3DX_FROM_FILE,
        D3DX_FROM_FILE,
        0,
        D3DFMT_UNKNOWN,
        D3DPOOL_MANAGED,
        D3DX_FILTER_TRIANGLE,
        D3DX_FILTER_TRIANGLE,
        D3DCOLOR_RGBA ( 0, 0, 0, 255 ),
        NULL,
        NULL,
        &mD3DTexture );
    //CXASSERT_RESULT_FALSE (
    //D3DXCreateTextureFromFileA ( D9DEVICE->GetDvc(), name, &mD3DTexture )
    //);
    return nullptr != mD3DTexture;
}

GTexture::~GTexture()
{
    dSafeRelease ( mD3DTexture );
}

void GTexture::destory()
{
    delete this;
}

void GTexture::set ( int stage )
{
    bool useTexture = mD3DTexture != nullptr;

    D9Device->GetDvc()->SetTexture ( stage,  mD3DTexture );

    D9Device->GetDvc()->SetTextureStageState ( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    D9Device->GetDvc()->SetTextureStageState ( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    D9Device->GetDvc()->SetTextureStageState ( 0, D3DTSS_COLOROP, useTexture ? D3DTOP_MODULATE : D3DTOP_SELECTARG2 );

    D9Device->GetDvc()->SetTextureStageState ( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    D9Device->GetDvc()->SetTextureStageState ( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
    D9Device->GetDvc()->SetTextureStageState ( 0, D3DTSS_ALPHAOP, useTexture ? D3DTOP_MODULATE : D3DTOP_SELECTARG2 );
}


