#include "GGameDemoHeader.h"
#include "GTexture.h"
#include "GD9Device.h"

GTexture::GTexture()
    : mD3DTexture ( 0 )
{

}

bool GTexture::createFromFile ( const char* name )
{
    dSafeRelease ( mD3DTexture );
    CXASSERT_RESULT_FALSE (
        D3DXCreateTextureFromFileA ( D9DEVICE->GetDvc(), name, &mD3DTexture )
    );
    return nullptr != mD3DTexture;
}

GTexture::~GTexture()
{
    //dSafeRelease ( mD3DTexture );
}
