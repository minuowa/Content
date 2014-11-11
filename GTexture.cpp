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
        D9DEVICE->GetDvc(),
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
    //dSafeRelease ( mD3DTexture );
}


